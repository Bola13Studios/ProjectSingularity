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

  if (m_currentRound >= rounds.Num())
  {
    if (isProgressive)
    {
      GenerateNextProgressiveRound();
    }
    else
    {
      UE_LOG(LogTemp, Warning, TEXT("all rounds are completed"));
      return;
    }
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

void ASpawnerManager::GenerateNextProgressiveRound()
{ // for now we just up the number of enemies to spawn each round
  FRoundSpawn baseRound = (rounds.Num() > 0) ? rounds[rounds.Num() - 1] : m_generatedRound;

  int32 newTotal = FMath::CeilToInt(baseRound.totalEnemies * difficultyMultiplier) + extraEnemiesPerRound;
  // @missing upping the enemy health
  // @missing upping the enemy damage

  m_generatedRound.totalEnemies = newTotal;
  m_generatedRound.info = baseRound.info;

  rounds.Add(m_generatedRound);

  UE_LOG(LogTemp, Warning, TEXT("New round generated: %d total enemies"), newTotal);
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
      if (m_spawnersReady < spawners.Num())
      {
        m_spawnersReady++;
        UE_LOG(LogTemp, Warning, TEXT("Spawner pronto: %d/%d"), m_spawnersReady, spawners.Num());

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

  FRoundSpawn currentRoundData;

  if (rounds.IsValidIndex(m_currentRound))
  {
    currentRoundData = rounds[m_currentRound];
  }
  else if (isProgressive)
  {
    currentRoundData = m_generatedRound;
    UE_LOG(LogTemp, Warning, TEXT("SetupSpawner: using progressive data (enemies: %d)"), currentRoundData.totalEnemies);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("SetupSpawner: no more rounds."));
    return;
  }

  _spawner->Setup(currentRoundData.totalEnemies, currentRoundData.info);
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
