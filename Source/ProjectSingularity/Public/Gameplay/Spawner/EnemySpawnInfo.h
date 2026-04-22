#pragma once

#include "CoreMinimal.h"
#include "EnemySpawnInfo.generated.h"

class ABaseEnemy;

USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere)
  TSubclassOf<ABaseEnemy> enemyClass;

  UPROPERTY(EditAnywhere)
  float weight = 100.0f;
};
