#include "Gameplay/Spawner/SpawnerManager.h"

ASpawnerManager::ASpawnerManager()
{
  PrimaryActorTick.bCanEverTick = false;
}

void ASpawnerManager::BeginPlay()
{
  Super::BeginPlay();

  UE_LOG(LogTemp, Error, TEXT("Using the deprecated version, please remove"));
}
