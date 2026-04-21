#include "ProjectSingularity/Public/Components/Hype/HypeSourceComponent.h"

int UHypeSourceComponent::GetHype()
{
  if (m_hasBeenHit) return false;

  // this delegate broadcast is temporary for testing only
  m_onHit.Broadcast();
  m_hasBeenHit = true;

  return m_currentHypeValue;
}
