#include "Gameplay/Spawner/SpawnerManager.h"
#include "Gameplay/Spawner/SpawnPoint.h"

ASpawnerManager::ASpawnerManager()
{
  PrimaryActorTick.bCanEverTick = false;

  timeBetweenRounds = 1.0f;

  rounds = {};
  spawners = {};

  m_currentRound = 0;
  m_spawnersOnHold = 0;
}

void ASpawnerManager::BeginPlay()
{
  Super::BeginPlay();

  for (TObjectPtr<ASpawnPoint> spawner : spawners)
  {
    if (!spawner) continue;

    spawner->OnSpawnPointStateChanged.AddDynamic(this, &ASpawnerManager::SpawnPointStateChanged);
  }
}

void ASpawnerManager::NextRound()
{
  for (TObjectPtr<ASpawnPoint> spawner : spawners)
  {
    if (!spawner) continue;

    spawner->StartSpawning();
  }
}

void ASpawnerManager::OnRoundEnd()
{
  OnStateChange.Broadcast(ESpawnManagerState::WAITING);

  m_currentRound++;

  m_spawnersOnHold = 0;

  if (m_currentRound >= rounds.Num())
  {
    //OnStateChange.Broadcast(ESpawnManagerState::FINISHED);
    return;
  }

  GetWorld()->GetTimerManager().ClearTimer(m_roundTimerHandle);
  GetWorld()->GetTimerManager().SetTimer(m_roundTimerHandle, this, &ASpawnerManager::StartNextRoundDelayed,
                                         timeBetweenRounds, false);
}

void ASpawnerManager::StartNextRoundDelayed()
{
  if (rounds.IsValidIndex(m_currentRound))
  {
    m_spawnersReady = 0;

    for (auto& spawner : spawners)
    {
      SetupSpawner(spawner);
    }
  }
}

void ASpawnerManager::OnRoundStart()
{
  m_spawnersReady = 0;

  OnStateChange.Broadcast(ESpawnManagerState::ONGOING);

  SetupRound();
}

void ASpawnerManager::SpawnPointStateChanged(ESpawnPointState _state, ASpawnPoint* _spawner)
{
  switch (_state)
  {
    case ESpawnPointState::IDLE:
      if (!m_initializedSpawners.Contains(_spawner))
      {
        SetupSpawner(_spawner);
        m_initializedSpawners.Add(_spawner);
      }
      break;

    case ESpawnPointState::ISREADY:
      m_spawnersReady++;
      if (m_spawnersReady == spawners.Num())
      {
        OnRoundStart();
      }
      break;

    case ESpawnPointState::WAITING:
      m_spawnersOnHold++;
      if (m_spawnersOnHold == spawners.Num())
      {
        OnRoundEnd();
      }
      break;

    case ESpawnPointState::SPAWNING:
    default:
      // unregistered state, @remind add log here
      break;
  }
}

void ASpawnerManager::SetupSpawner(TObjectPtr<ASpawnPoint> _spawner)
{
  if (!_spawner) return;
  if (rounds.Num() == 0)
  {
    UE_LOG(LogTemp, Warning, TEXT("SpawnerManager: No rounds configured."));
    return;
  }
  if (!rounds.IsValidIndex(m_currentRound)) return;

  _spawner->Setup(true, rounds[m_currentRound].totalEnemies, rounds[m_currentRound].info);
}

void ASpawnerManager::SetupRound()
{
  for (auto& spawner : spawners)
  {
    SetupSpawner(spawner);
  }

  NextRound();
}
