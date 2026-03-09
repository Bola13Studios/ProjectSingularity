#include "Gameplay/Spawner/EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "ProjectSingularity/Public/Gameplay/Character/Enemy/BaseEnemy.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_spawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = m_spawnArea;
}

void AEnemySpawner::SpawnEnemy()
{
  if (GetWorld() && m_enemyType)
  {
    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (NavSystem)
    {
      FNavLocation RandomLocation;
      FVector RandomPointInBox = UKismetMathLibrary::RandomPointInBoundingBox(m_spawnArea->Bounds.Origin, m_spawnArea->Bounds.BoxExtent);
      FVector QueryExtent = FVector(1000.0f, 1000.0f, 1000.0f);

      if (NavSystem->ProjectPointToNavigation(RandomPointInBox, RandomLocation, QueryExtent))
      {
        ABaseEnemy* DefaultEnemy = m_enemyType->GetDefaultObject<ABaseEnemy>();
        FVector FinalLocation = RandomLocation.Location;

        if (DefaultEnemy && DefaultEnemy->GetCapsuleComponent())
        {
          
          float HalfHeight = DefaultEnemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
          FinalLocation.Z += HalfHeight;
        }

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        if (GetWorld()->SpawnActor<ABaseEnemy>(m_enemyType, FinalLocation, GetActorRotation(), SpawnParams))
        {
          m_enemiesSpawned++;

          if (m_enemiesSpawned >= m_enemyCount)
          {
            GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

            if (m_reloadSpawn)
            {
              FTimerHandle ResetTimerHandle;
              GetWorldTimerManager().SetTimer(ResetTimerHandle, this, &AEnemySpawner::ResetSpawner, m_waveCooldown, false);
            }
          }
        }
      }
    }
  }
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (m_enemyCount > 0)
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, m_spawnDelay, true);
	}
}

void AEnemySpawner::ResetSpawner()
{
  m_enemiesSpawned = 0;

  GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, m_spawnDelay, true,0.0f);
}


