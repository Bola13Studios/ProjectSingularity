#include "AI/Tasks/BTTask_Attack.h"
#include "Gameplay/Character/Enemy/BaseEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
  NodeName = "Attack";
  bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  if (UObject* ObjOwner = OwnerComp.GetBlackboardComponent()->GetValueAsObject(selfActorKey.SelectedKeyName))
  {
    if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(ObjOwner))
    {
      if (UObject* ObjTarget = OwnerComp.GetBlackboardComponent()->GetValueAsObject(targetKey.SelectedKeyName))
      {
        if (AActor* Target = Cast<AActor>(ObjTarget))
        {
          Enemy->Attack(Target);
        }
      }
    }
  }
  return EBTNodeResult::Type();
}
