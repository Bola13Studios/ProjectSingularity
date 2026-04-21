#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTDecorator_DistanceCheck.generated.h"

UCLASS()
class PROJECTSINGULARITY_API UBTDecorator_DistanceCheck : public UBTDecorator
{
  GENERATED_BODY()

public:
  UBTDecorator_DistanceCheck();

  UPROPERTY(EditAnywhere, Category = "Condition")
  FBlackboardKeySelector ownerKey;

  UPROPERTY(EditAnywhere, Category = "Condition")
  FBlackboardKeySelector targetKey;

  UPROPERTY(EditAnywhere, Category = "Condition")
  FBlackboardKeySelector minDistance;

  UPROPERTY(EditAnywhere, Category = "Condition")
  FBlackboardKeySelector maxDistance;

protected:
  FVector GetLocationFromBB(const UBlackboardComponent* BB, const FBlackboardKeySelector& Key) const;

  virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
