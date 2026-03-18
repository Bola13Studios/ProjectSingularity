#include "ProjectSingularity/Public/Components/Hype/HypeSourceComponent.h"

int UHypeSourceComponent::GetHype() const
{
  return FMath::CeilToInt(BaseHypeValue * BaseMultiplier);
}
