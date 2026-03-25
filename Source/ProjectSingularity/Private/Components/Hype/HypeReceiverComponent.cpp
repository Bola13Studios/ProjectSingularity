#include "ProjectSingularity/Public/Components/Hype/HypeReceiverComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeSourceComponent.h"
#include "ProjectSingularity/Public/Data/DataTable/HypeMultipliers.h"
#include "ProjectSingularity/Public/Data/DataTable/HypeLevels.h"

void UHypeReceiverComponent::RegisterKill(UHypeSourceComponent* _Source, const bool& _Critical, const int& _MultiKill)
{
  m_CurrentKillStreak++;

  // basic implementation, must be improved
  TArray<FHypeMultipliers*> HypeMultipliers;
  if (IsValid(m_HypeMultiplierTable) && _Critical)
  {
    m_HypeMultiplierTable->GetAllRows(TEXT("Multipliers"), HypeMultipliers);
    if (HypeMultipliers.IsValidIndex(0))
    {
      AddHype(_Source->GetHype() * HypeMultipliers[0]->CriticalMultiplier);
    }
  }

  AddHype(_Source->GetHype());
}

void UHypeReceiverComponent::UpdateHypeLevel()
{
  // basic implementation
  TArray<FHypeLevels*> HypeLevels;
  if (IsValid(m_HypeLevelTable))
  {
    m_HypeLevelTable->GetAllRows(TEXT("Levels"), HypeLevels);
    for (const auto& Level : HypeLevels)
    {
      if (m_CurrentHypeValue >= Level->MinRequiredHype && m_CurrentHypeValue <= Level->MaxRequiredHype)
      {
        m_CurrentHypeLevel = Level->Level;
        break;
      }
    }
  }
}

bool UHypeReceiverComponent::IsHypeEnough(float _Amount)
{
  if (m_CurrentHypeValue >= _Amount) return true;
  return false;
}
