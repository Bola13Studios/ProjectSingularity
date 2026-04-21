// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/BTTask_VoxelMoveTo.h"
#include "Gameplay/Character/Enemy/ChildEnemies/StreamerEnemy.h"
#include "Systems/VoxelNavigationSubsystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_VoxelMoveTo::UBTTask_VoxelMoveTo()
{
  NodeName = "Voxel Move To";
  bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_VoxelMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  owner = Cast<AStreamerEnemy>(OwnerComp.GetAIOwner()->GetPawn());

  if (owner == NULL)
  {
    return EBTNodeResult::Failed;
  }

  return TryGetPath(OwnerComp);
}

void UBTTask_VoxelMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
  if (!owner)
  {
    FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
  }

  FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UBTTask_VoxelMoveTo::TryGetPath(UBehaviorTreeComponent& OwnerComp)
{
  TArray<FVector> path = FindPath(OwnerComp);
  
  if (path.Num() < 2)
  {
    return EBTNodeResult::Succeeded;
  }

  owner->SetPath(path);
  return EBTNodeResult::InProgress;
}

TArray<FVector> UBTTask_VoxelMoveTo::FindPath(UBehaviorTreeComponent& OwnerComp)
{
  TArray<FVector> path;

  if (!owner)
  {
    return path;
  }

  UWorld* world = owner->GetWorld();
  UVoxelNavigationSubsystem* nav = world->GetSubsystem<UVoxelNavigationSubsystem>();
  FVector target;

  if (UObject* obj = OwnerComp.GetBlackboardComponent()->GetValueAsObject(targetKey.SelectedKeyName))
  {
    if (AActor* actor = Cast<AActor>(obj))
    {
      target = actor->GetActorLocation();
    }
  }
  else
  {
    target = OwnerComp.GetBlackboardComponent()->GetValueAsVector(targetKey.SelectedKeyName);
  }

  //Consult with design
  FVector offset = ((target - owner->GetActorLocation()).GetSafeNormal() + FVector::UpVector).GetSafeNormal()
      * OwnerComp.GetBlackboardComponent()->GetValueAsFloat(targetOffsetKey.SelectedKeyName);

  path = nav->FindPath(owner->GetActorLocation(), target + offset);
  path = nav->OptimizePath(path, world);
  path.RemoveAt(0);
  return path;
}
