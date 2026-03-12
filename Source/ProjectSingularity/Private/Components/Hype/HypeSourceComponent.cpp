#include "HypeSourceComponent.h"

int UHypeSourceComponent::GetHype() const
{
  return FMath::CeilToInt(BaseHypeValue * BaseMultiplier);
}
