#include "Gameplay/Spawner/SpawnPoint.h"
#include "Gameplay/Spawner/EnemySpawnInfo.h"
#include "Gameplay/Character/Enemy/BaseEnemy.h"

ASpawnPoint::ASpawnPoint()
{
  PrimaryActorTick.bCanEverTick = false;

  spawnVariation = FVector2D(10.0f, 10.0f);

  isAssigned = false;

  m_currentSpawnedEntities = 0;
  m_totalEntitiesToSpawn = 0;
  m_enemyTypes = {};
}

void ASpawnPoint::BeginPlay()
{
  Super::BeginPlay();

  OnSpawnPointStateChanged.Broadcast(ESpawnPointState::IDLE, this);
}

void ASpawnPoint::EnemyDied()
{
  if (m_currentSpawnedEntities > 0) --m_currentSpawnedEntities;

  if (m_currentSpawnedEntities == 0 && m_spawnedSoFar >= m_totalEntitiesToSpawn)
  {
    OnSpawnPointStateChanged.Broadcast(ESpawnPointState::WAITING, this);
  }
}

void ASpawnPoint::StartSpawning()
{
  m_currentSpawnedEntities = 0;
  m_spawnedSoFar = 0;

  OnSpawnPointStateChanged.Broadcast(ESpawnPointState::SPAWNING, this);

  GetWorld()->GetTimerManager().SetTimer(m_spawnTimerHandle, this, &ASpawnPoint::SpawnTick, spawnInterval, true);
}

void ASpawnPoint::SpawnTick()
{
  int32 spawnCount = 0;

  while (spawnCount < spawnPerTick && m_spawnedSoFar < m_totalEntitiesToSpawn)
  {
    auto classToSpawn = GetRandomEnemyClass();
    if (!classToSpawn)
    {
      ++m_spawnedSoFar;
      continue;
    }

    FVector origin = GetActorLocation();

    float x = FMath::FRandRange(origin.X - spawnVariation.X, origin.X + spawnVariation.X);
    float y = FMath::FRandRange(origin.Y - spawnVariation.Y, origin.Y + spawnVariation.Y);
    float z = origin.Z;

    ABaseEnemy* enemy = GetWorld()->SpawnActor<ABaseEnemy>(classToSpawn, FVector(x, y, z), GetActorRotation());

    if (enemy)
    {
      ++m_currentSpawnedEntities;
      enemy->OnEnemyDeath.AddUObject(this, &ASpawnPoint::EnemyDied);
    }

    ++m_spawnedSoFar;
    ++spawnCount;
  }

  if (m_spawnedSoFar >= m_totalEntitiesToSpawn)
  {
    GetWorld()->GetTimerManager().ClearTimer(m_spawnTimerHandle);
  }
}

void ASpawnPoint::Setup(bool _assigned, int32 _totalEnemies, const TArray<FEnemySpawnInfo>& _enemyTypes)
{
  m_totalEntitiesToSpawn = _totalEnemies;
  m_enemyTypes = _enemyTypes;

  isAssigned = _assigned;

  OnSpawnPointStateChanged.Broadcast(ESpawnPointState::ISREADY, this);
}

TSubclassOf<ABaseEnemy> ASpawnPoint::GetRandomEnemyClass() const
{
  float totalWeight = 0.0f;

  for (const auto& type : m_enemyTypes)
  {
    if (!type.enemyClass) continue;
    totalWeight += type.weight;
  }

  if (totalWeight <= 0.f) return nullptr;

  float random = FMath::FRandRange(0.0f, totalWeight);

  float accumulated = 0.0f;

  for (const auto& type : m_enemyTypes)
  {
    if (!type.enemyClass) continue;

    accumulated += type.weight;

    if (random <= accumulated)
    {
      return type.enemyClass;
    }
  }

  return nullptr;
}
