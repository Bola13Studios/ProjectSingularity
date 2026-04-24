#include "Gameplay/Spawner/SpawnPoint.h"
#include "Gameplay/Spawner/EnemySpawnInfo.h"
#include "Gameplay/Character/Enemy/BaseEnemy.h"
#include "ProjectSingularity/Public/Systems/SpawnManager.h"

ASpawnPoint::ASpawnPoint()
{
  PrimaryActorTick.bCanEverTick = false;

  spawnVariation = FVector2D(100.0f, 100.0f);

  m_currentSpawnedEntities = 0;
  m_totalEntitiesToSpawn = 0;
}

void ASpawnPoint::BeginPlay()
{
  Super::BeginPlay();

  if (UWorld* world = GetWorld())
  {
    if (USpawnManager* manager = world->GetSubsystem<USpawnManager>())
    {
      manager->RegisterSpawnPoint(this);
    }
  }
}

void ASpawnPoint::EnemyDied()
{
  if (m_currentSpawnedEntities > 0) --m_currentSpawnedEntities;

  // if all the enemies died we change the status to waiting
  if (m_currentSpawnedEntities <= 0 && m_spawnedSoFar >= m_totalEntitiesToSpawn)
  {
    m_currentSpawnedEntities = 0;
    OnSpawnPointStateChanged.Broadcast(ESpawnPointState::WAITING, this);
  }
}

void ASpawnPoint::StartSpawning()
{
  m_currentSpawnedEntities = 0;
  m_spawnedSoFar = 0;

  OnSpawnPointStateChanged.Broadcast(ESpawnPointState::SPAWNING, this);

  // starting a timer to avoind spawning all the enemies at the same time
  GetWorld()->GetTimerManager().SetTimer(m_spawnTimerHandle, this, &ASpawnPoint::SpawnTick, spawnInterval, true);
}

void ASpawnPoint::SpawnTick()
{
  int32 spawnCount = 0;

  while (spawnCount < spawnPerTick && m_spawnedSoFar < m_totalEntitiesToSpawn)
  {
    TSubclassOf<ABaseEnemy> enemyClass = m_enemyType;
    if (!enemyClass) return;

    bool bSpawned = false;
    int32 retryAttempts = 0;
    const int32 MAX_RETRIES = 5;

    while (!bSpawned && retryAttempts < MAX_RETRIES)
    {
      FVector origin = GetActorLocation();
      float x = FMath::FRandRange(origin.X - spawnVariation.X, origin.X + spawnVariation.X);
      float y = FMath::FRandRange(origin.Y - spawnVariation.Y, origin.Y + spawnVariation.Y);
      float z = origin.Z + 50.0f;

      FActorSpawnParameters SpawnParams;

      SpawnParams.SpawnCollisionHandlingOverride =
          ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

      ABaseEnemy* enemy =
          GetWorld()->SpawnActor<ABaseEnemy>(enemyClass, FVector(x, y, z), GetActorRotation(), SpawnParams);

      if (enemy)
      {
        ++m_currentSpawnedEntities;
        enemy->OnEnemyDeath.AddUFunction(this, FName("EnemyDied"));

        ++m_spawnedSoFar;
        ++spawnCount;
        bSpawned = true;
      }
      else
      {
        retryAttempts++;
        UE_LOG(LogTemp, Warning, TEXT("%s: Spawn failed [%d/%d]"), *GetName(), retryAttempts,
               MAX_RETRIES);
      }
    }

    if (!bSpawned)
    {
      UE_LOG(LogTemp, Error, TEXT("%s: Unable to find spot after %d tries!"), *GetName(), MAX_RETRIES);
      break;
    }
  }

  if (m_spawnedSoFar >= m_totalEntitiesToSpawn)
  {
    GetWorld()->GetTimerManager().ClearTimer(m_spawnTimerHandle);

    if (m_currentSpawnedEntities <= 0)
    {
      OnSpawnPointStateChanged.Broadcast(ESpawnPointState::WAITING, this);
    }
  }
}

void ASpawnPoint::Setup(int32 _roundIndex)
{
  if (rounds.IsValidIndex(_roundIndex))
  {
    m_totalEntitiesToSpawn = rounds[_roundIndex].totalEnemies;
    m_enemyType = rounds[_roundIndex].enemyClass;

    OnSpawnPointStateChanged.Broadcast(ESpawnPointState::ISREADY, this);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("SpawnPoint %s does not have a configuration for the round %d!"), *GetName(),
           _roundIndex);
    OnSpawnPointStateChanged.Broadcast(ESpawnPointState::FINISHED, this);
  }
}
