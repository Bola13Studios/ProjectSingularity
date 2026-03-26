#include "ProjectSingularity/Public/Components/Hype/HypeComponent.h"

UHypeComponent::UHypeComponent()
    : m_CurrentHypeValue(10)
{
  PrimaryComponentTick.bCanEverTick = true;
}

void UHypeComponent::AddHype(const int& _Points)
{
  m_CurrentHypeValue += _Points;
  OnHypeChanged.Broadcast(m_CurrentHypeValue, _Points);
}

int UHypeComponent::GetHype()
{
  return m_CurrentHypeValue;
}

void UHypeComponent::ResetHype()
{
  const int OldValue = m_CurrentHypeValue;
  m_CurrentHypeValue = 0;

  OnHypeChanged.Broadcast(m_CurrentHypeValue, -OldValue);
}
