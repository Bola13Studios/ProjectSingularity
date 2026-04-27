/************************************************************************
 * @description: This class defines the spawner manager for the spawn points
 * @author: Josephine Esposito
 * @date: 17/04/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerManager.generated.h"

/**
 * @brief This class defines the spawner manager for the spawn points
 */
UCLASS()
class PROJECTSINGULARITY_API ASpawnerManager : public AActor
{
  GENERATED_BODY()

public:
  /**
   * @brief Sets default values for this actor's properties
   */
  ASpawnerManager();

protected:
  /**
   * @brief Called when the game starts or when spawned
   */
  virtual void BeginPlay() override;
};
