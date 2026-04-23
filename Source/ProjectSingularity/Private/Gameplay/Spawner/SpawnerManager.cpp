#include "Gameplay/Spawner/SpawnerManager.h"
#include "Gameplay/Spawner/SpawnPoint.h"

ASpawnerManager::ASpawnerManager()
{
  PrimaryActorTick.bCanEverTick = false;

  timeBetweenRounds = 1.0f;

  spawners = {};

  m_currentRound = 0;
  m_spawnersOnHold = 0;
}

void ASpawnerManager::BeginPlay()
{
  Super::BeginPlay();

  for (TObjectPtr<ASpawnPoint> spawner : spawners)
  {
    if (spawner)
    {
      // binding to each spawn state to handle the rounds
      spawner->OnSpawnPointStateChanged.AddDynamic(this, &ASpawnerManager::SpawnPointStateChanged);

      // we setup the spawners
      if (!m_initializedSpawners.Contains(spawner.Get()))
      {
        m_initializedSpawners.Add(spawner.Get());
        SetupSpawner(spawner);
      }
    }
  }
}

void ASpawnerManager::NextRound()
{
  UE_LOG(LogTemp, Warning, TEXT("Next round called"));
  for (TObjectPtr<ASpawnPoint> spawner : spawners)
  {
    if (!spawner) continue;

    // starting spawning
    spawner->StartSpawning();
  }
}

void ASpawnerManager::OnRoundEnd()
{
  OnStateChange.Broadcast(ESpawnManagerState::WAITING);
  m_currentRound++;
  m_spawnersOnHold = 0;
  m_spawnersReady = 0;

  int32 finishedSpawners = 0;
  for (auto& spawner : spawners)
  {
    if (spawner && !spawner->rounds.IsValidIndex(m_currentRound))
    {
      finishedSpawners++;
    }
  }

  if (finishedSpawners == spawners.Num())
  {
    UE_LOG(LogTemp, Warning, TEXT("All the spawners have finished their rounds"));
    if (!isProgressive)
    {
      OnStateChange.Broadcast(ESpawnManagerState::FINISHED);
      return;
    }
    m_currentRound = 0;
  }

  GetWorld()->GetTimerManager().SetTimer(m_roundTimerHandle, this, &ASpawnerManager::StartNextRoundDelayed,
                                         timeBetweenRounds, false);
}

void ASpawnerManager::StartNextRoundDelayed()
{
  UE_LOG(LogTemp, Warning, TEXT("setting the delayed round"));

  m_spawnersReady = 0;
  m_spawnersOnHold = 0;

  for (auto& spawner : spawners)
  {
    if (spawner)
    {
      SetupSpawner(spawner);
    }
  }
}

void ASpawnerManager::OnRoundStart()
{
  UE_LOG(LogTemp, Warning, TEXT("on round start called"));
  m_spawnersReady = 0;

  OnStateChange.Broadcast(ESpawnManagerState::ONGOING);

  NextRound();
}

void ASpawnerManager::SpawnPointStateChanged(ESpawnPointState _state, ASpawnPoint* _spawner)
{
  if (!_spawner) return;

  UE_LOG(LogTemp, Warning, TEXT("spawnpoint state changed"));
  switch (_state)
  {
    case ESpawnPointState::IDLE:
      if (!m_initializedSpawners.Contains(_spawner))
      {
        m_initializedSpawners.Add(_spawner);
        SetupSpawner(_spawner);
      }
      break;

    case ESpawnPointState::ISREADY:
    case ESpawnPointState::FINISHED:
      if (m_spawnersReady < spawners.Num())
      {
        m_spawnersReady++;
        UE_LOG(LogTemp, Warning, TEXT("Spawner ready: %d/%d"), m_spawnersReady, spawners.Num());

        if (m_spawnersReady == spawners.Num())
        {
          OnRoundStart();
        }
      }
      break;

    case ESpawnPointState::WAITING:
      m_spawnersOnHold++;
      UE_LOG(LogTemp, Warning, TEXT("spawner is waiting"));
      if (m_spawnersOnHold == spawners.Num())
      {
        OnRoundEnd();
      }
      break;

    case ESpawnPointState::SPAWNING:
      UE_LOG(LogTemp, Warning, TEXT("spawner is spawning"));
      break;
    default:
      // unregistered state, @remind add log here
      break;
  }
}

void ASpawnerManager::SetupSpawner(TObjectPtr<ASpawnPoint> _spawner)
{
  if (!_spawner) return;

  _spawner->Setup(m_currentRound);
}

void ASpawnerManager::SetupRound()
{
  for (auto& spawner : spawners)
  {
    if (!spawner) continue;

    SetupSpawner(spawner);
  }

  NextRound();
}
