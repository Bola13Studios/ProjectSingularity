#include "ProjectSingularity/Public/Components/Hype/HypeSourceComponent.h"

int UHypeSourceComponent::GetHype()
{
  if (m_hasBeenHit) return false;

  // this delegate broadcast is temporary for testing only
  m_onHit.Broadcast();
  m_hasBeenHit = true;

  if (!m_isWeakPoint) return m_CurrentHypeValue;

  return FMath::CeilToInt(m_CurrentHypeValue + (m_CurrentHypeValue * m_baseMultiplier));
}
