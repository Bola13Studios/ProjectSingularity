#include "HypeReceiverComponent.h"
#include "HypeSourceComponent.h"
#include "ProjectSingularity/Data/DataTable/HypeMultipliers.h"
#include "ProjectSingularity/Data/DataTable/HypeLevels.h"

void UHypeReceiverComponent::RegisterKill(UHypeSourceComponent* _Source, const bool& _Critical, const int& _MultiKill)
{
  CurrentKillStreak++;

  TArray<FHypeMultipliers*> HypeMultipliers;
  if (IsValid(HypeMultiplierTable) && _Critical)
  {
    HypeMultiplierTable->GetAllRows(TEXT("Multipliers"), HypeMultipliers);
    if (HypeMultipliers.IsValidIndex(0))
    {
      AddHype(_Source->GetHype() * HypeMultipliers[0]->CriticalMultiplier);
    }
  }
  
  AddHype(_Source->GetHype());
}

void UHypeReceiverComponent::UpdateHypeLevel()
{
  // @remind missing logic about changind hype level here
  TArray<FHypeLevels*> HypeLevels;
  if (IsValid(HypeLevelTable))
  {
    HypeLevelTable->GetAllRows(TEXT("Levels"), HypeLevels);
    for (const auto& Level : HypeLevels)
    {
      if (BaseHypeValue >= Level->MinRequiredHype && BaseHypeValue <= Level->MaxRequiredHype)
      {
        CurrentHypeLevel = Level->Level;
        break;
      }
    }
  }
}
