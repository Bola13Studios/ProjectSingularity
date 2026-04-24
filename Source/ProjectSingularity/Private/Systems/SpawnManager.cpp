#include "Systems/SpawnManager.h"
#include "Gameplay/Spawner/SpawnPoint.h"
/*
#include "ProjectSingularity/Public/Systems/GameManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
*/

void USpawnManager::Initialize(FSubsystemCollectionBase& _rCollection)
{
  timeBetweenRounds = 1.0f;
  spawners.Empty();
  m_currentRound = 0;
  m_spawnersOnHold = 0;
  m_currentState = ESpawnManagerState::WAITING;

  /*
  for (TObjectPtr<ASpawnPoint> spawner : spawners)
  {
    if (spawner)
    {
      // binding to each spawn state to handle the rounds
      spawner->OnSpawnPointStateChanged.AddDynamic(this, &USpawnManager::SpawnPointStateChanged);

      // we setup the spawners
      if (!m_initializedSpawners.Contains(spawner.Get()))
      {
        m_initializedSpawners.Add(spawner.Get());
        SetupSpawner(spawner);
      }
    }
  }
  */
}

void USpawnManager::Deinitialize()
{
  for (TObjectPtr<ASpawnPoint> spawner : spawners)
  {
    if (spawner)
    {
      // unbinding to each spawn state to handle the rounds
      spawner->OnSpawnPointStateChanged.RemoveAll(this);
    }
  }

  m_currentState = ESpawnManagerState::FINISHED;
}

void USpawnManager::RegisterSpawnPoint(TObjectPtr<ASpawnPoint> _inSpawner)
{
  if (!IsValid(_inSpawner)) return;

  spawners.Add(_inSpawner);

  _inSpawner->OnSpawnPointStateChanged.AddDynamic(this, &USpawnManager::SpawnPointStateChanged);

  if (!m_initializedSpawners.Contains(_inSpawner))
  {
    m_initializedSpawners.Add(_inSpawner);
  }

  UE_LOG(LogTemp, Warning, TEXT("Spawnpoint %s successfully registered!"), *_inSpawner->GetName());
}

void USpawnManager::Activate(float _delay)
{
  // no registered spawners
  if (spawners.Num() == 0)
  {
    UE_LOG(LogTemp, Warning, TEXT("No registered spawners??"));
    return;
  }
  // rounds already started
  if (m_currentState == ESpawnManagerState::ONGOING) return;

  GetWorld()->GetTimerManager().ClearTimer(m_roundTimerHandle);
  GetWorld()->GetTimerManager().SetTimer(m_roundTimerHandle, this, &USpawnManager::SetupRound, _delay, false);
}

void USpawnManager::NextRound()
{
  UE_LOG(LogTemp, Warning, TEXT("Next round called"));
  for (TObjectPtr<ASpawnPoint> spawner : spawners)
  {
    if (!spawner) continue;

    // starting spawning
    spawner->StartSpawning();
  }
}

void USpawnManager::OnRoundEnd()
{
  OnStateChange.Broadcast(ESpawnManagerState::WAITING);
  m_currentState = ESpawnManagerState::WAITING;

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
      m_currentState = ESpawnManagerState::FINISHED;
      OnStateChange.Broadcast(ESpawnManagerState::FINISHED);

      /* // missing win/lost condition so this is temporary disabled 
      if (UWorld* World = GetWorld())
      {
        if (UGameInstance* GI = World->GetGameInstance())
        {
          UGameManagerSubsystem* GameManager = GI->GetSubsystem<UGameManagerSubsystem>();
          if (GameManager)
          {
            // GameManager->SetGameState(EGameState::VICTORY);
            GameManager->SetGameState();
          }
        }
      }
      */

      return;
    }
    m_currentRound = 0;
  }

  GetWorld()->GetTimerManager().ClearTimer(m_roundTimerHandle);
  GetWorld()->GetTimerManager().SetTimer(m_roundTimerHandle, this, &USpawnManager::StartNextRoundDelayed,
                                         timeBetweenRounds, false);
}

void USpawnManager::OnRoundStart()
{
  UE_LOG(LogTemp, Warning, TEXT("on round start called"));
  m_spawnersReady = 0;

  OnStateChange.Broadcast(ESpawnManagerState::ONGOING);
  m_currentState = ESpawnManagerState::ONGOING;

  NextRound();
}

void USpawnManager::SpawnPointStateChanged(ESpawnPointState _state, ASpawnPoint* _spawner)
{
  if (!_spawner) return;

  //UE_LOG(LogTemp, Warning, TEXT("spawnpoint state changed"));
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
        UE_LOG(LogTemp, Log, TEXT("Spawner ready: %d/%d"), m_spawnersReady, spawners.Num());

        if (m_spawnersReady == spawners.Num())
        {
          OnRoundStart();
        }
      }
      break;

    case ESpawnPointState::WAITING:
      m_spawnersOnHold++;
      //UE_LOG(LogTemp, Warning, TEXT("spawner is waiting"));
      if (m_spawnersOnHold == spawners.Num())
      {
        OnRoundEnd();
      }
      break;

    case ESpawnPointState::SPAWNING:
      //UE_LOG(LogTemp, Warning, TEXT("spawner is spawning"));
      break;
    default:

      UE_LOG(LogTemp, Error, TEXT("Unhandled state in SpawnManager"));
      break;
  }
}

void USpawnManager::SetupSpawner(TObjectPtr<ASpawnPoint> _spawner) const
{
  if (!_spawner) return;

  _spawner->Setup(m_currentRound);
}

void USpawnManager::SetupRound()
{
  for (auto& spawner : spawners)
  {
    if (!spawner) continue;

    SetupSpawner(spawner);
  }

  NextRound();
}

void USpawnManager::StartNextRoundDelayed()
{
  //UE_LOG(LogTemp, Warning, TEXT("setting the delayed round"));

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
