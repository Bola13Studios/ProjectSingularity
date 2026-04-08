#include "Components/WeakPointComponent.h"
#include "ProjectSingularity/Public/Systems/BaseGameInstance.h"
#include "ProjectSingularity/Public/Data/EnemyConfigDataAsset.h"
#include <Engine/World.h>

float UWeakPointComponent::GetDamageMultiplier() const
{
  if (UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance()))
  {
    if (!gameInstance->m_EnemyConfigDataAsset)
    {
      UE_LOG(
          LogTemp, Error,
          TEXT("Unable to retreive the EnemyConfigDataAsset from the BaseGameInstance. Are you sure it was assigned?"));
      return 1.0f;
    }
    return gameInstance->m_EnemyConfigDataAsset->weakpointDmgMultiplier;
  }
  return 1.0f;
}
