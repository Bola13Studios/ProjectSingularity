#include "Gameplay/Spawner/EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_spawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = m_spawnArea;
}

void AEnemySpawner::SpawnEnemy()
{
	if (GetWorld() && m_enemyType)
	{
		
		FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(m_spawnArea->Bounds.Origin, m_spawnArea->Bounds.BoxExtent);

		FRotator SpawnRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<ABaseEnemy>(m_enemyType, SpawnLocation, SpawnRotation, SpawnParams);

	}
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

