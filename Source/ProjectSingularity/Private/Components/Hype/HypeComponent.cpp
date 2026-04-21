#include "ProjectSingularity/Public/Components/Hype/HypeComponent.h"

UHypeComponent::UHypeComponent()
    : m_currentHypeValue(10)
{
  PrimaryComponentTick.bCanEverTick = true;
}

void UHypeComponent::AddHype(const int& _points)
{
  m_currentHypeValue += _points;
  onHypeChanged.Broadcast(m_currentHypeValue, _points);
}

int UHypeComponent::GetHype()
{
  return m_currentHypeValue;
}

void UHypeComponent::ResetHype()
{
  const int oldValue = m_currentHypeValue;
  m_currentHypeValue = 0;

  onHypeChanged.Broadcast(m_currentHypeValue, -oldValue);
}
