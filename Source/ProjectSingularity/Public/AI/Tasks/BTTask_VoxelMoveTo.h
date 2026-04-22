// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_VoxelMoveTo.generated.h"

/**
 * 
 */

class AStreamerEnemy;
UCLASS()
class PROJECTSINGULARITY_API UBTTask_VoxelMoveTo : public UBTTaskNode
{
  GENERATED_BODY()

public:
  UBTTask_VoxelMoveTo();

  UPROPERTY(EditAnywhere)
  FBlackboardKeySelector targetKey;

  UPROPERTY(EditAnywhere)
  FBlackboardKeySelector targetOffsetKey;

private:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
  virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                          float DeltaSeconds) override;

  EBTNodeResult::Type TryGetPath(UBehaviorTreeComponent& OwnerComp);
  TArray<FVector> FindPath(UBehaviorTreeComponent& OwnerComp);
  AStreamerEnemy* owner;
};
