#include "ProjectSingularity/Public/Components/Hype/HypeReceiverComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeSourceComponent.h"
#include "ProjectSingularity/Public/Data/DataTable/HypeMultipliers.h"
#include "ProjectSingularity/Public/Data/DataTable/HypeLevels.h"

void UHypeReceiverComponent::RegisterKill(UHypeSourceComponent* _source, const bool& _critical, const int& _multiKill)
{
  m_currentKillStreak++;

  // basic implementation, must be improved
  TArray<FHypeMultipliers*> hypeMultipliers;
  if (IsValid(m_hypeMultiplierTable) && _critical)
  {
    m_hypeMultiplierTable->GetAllRows(TEXT("Multipliers"), hypeMultipliers);
    if (hypeMultipliers.IsValidIndex(0))
    {
      AddHype(_source->GetHype() * hypeMultipliers[0]->CriticalMultiplier);
    }
  }

  AddHype(_source->GetHype());
}

void UHypeReceiverComponent::UpdateHypeLevel()
{
  // basic implementation
  TArray<FHypeLevels*> hypeLevels;
  if (IsValid(m_hypeLevelTable))
  {
    m_hypeLevelTable->GetAllRows(TEXT("Levels"), hypeLevels);
    for (const auto& level : hypeLevels)
    {
      if (m_currentHypeValue >= level->MinRequiredHype && m_currentHypeValue <= level->MaxRequiredHype)
      {
        m_currentHypeLevel = level->Level;
        break;
      }
    }
  }
}

bool UHypeReceiverComponent::IsHypeEnough(float _amount)
{
  if (m_currentHypeValue >= _amount) return true;
  return false;
}
