#include "AI/Decorators/BTDecorator_DistanceCheck.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTDecorator_DistanceCheck::UBTDecorator_DistanceCheck()
{
  NodeName = "Distance In Range";
}

FVector UBTDecorator_DistanceCheck::GetLocationFromBB(const UBlackboardComponent* BB,
                                                      const FBlackboardKeySelector& Key) const
{
  if (!BB) return FVector::ZeroVector;

  if (UObject* Obj = BB->GetValueAsObject(Key.SelectedKeyName))
  {
    if (AActor* Actor = Cast<AActor>(Obj))
    {
      return Actor->GetActorLocation();
    }
  }

  return BB->GetValueAsVector(Key.SelectedKeyName);
}

bool UBTDecorator_DistanceCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
  const UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
  if (!blackboard) return false;

  FVector owner = GetLocationFromBB(blackboard, ownerKey);
  FVector target = GetLocationFromBB(blackboard, targetKey);

  float distance = FVector::Distance(owner, target);

  float min = blackboard->GetValueAsFloat(minDistance.SelectedKeyName);
  float max = blackboard->GetValueAsFloat(maxDistance.SelectedKeyName);

  return distance >= min && distance <= max;
}
