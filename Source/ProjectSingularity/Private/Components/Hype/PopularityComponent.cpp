#include "Components/Hype/PopularityComponent.h"
#include "Components/Hype/HypeLevels.h"
#include "Systems/BaseGameInstance.h"
#include <Engine/World.h>

UPopularityComponent::UPopularityComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
}

void UPopularityComponent::BeginPlay()
{
  Super::BeginPlay();
}

void UPopularityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // @remind decreasing the popularity value with time
}

float UPopularityComponent::GetCurrentPopularity() const
{
  return m_currentPopularity;
}

int UPopularityComponent::GetPopularityLevel() const
{
  return m_hypeLevel->level;
}

void UPopularityComponent::IncreasePopularity(float _value)
{
  m_currentPopularity += _value;

  // we then check if we surpassed the current level max limit
  UpdateLevel();
}

void UPopularityComponent::DecreasePopularity(float _value)
{
  m_currentPopularity -= _value;

  // we then check if we surpassed the current level min limit
  UpdateLevel();
}

void UPopularityComponent::UpdateLevel()
{
  if (UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance()))
  {
    TArray<FHypeLevels*> hypeLevels;

    if (!IsValid(gameInstance->m_hypePopularityDataTable))
    {
      UE_LOG(
          LogTemp, Error,
          TEXT("Unable to retreive the PopularityDataTable from the BaseGameInstance. Are you sure it was assigned?"));
      return;
    }

    int32 newLevel = m_hypeLevel->level;
    int32 minValue = 0;
    int32 maxValue = 0;
    gameInstance->m_hypePopularityDataTable->GetAllRows(TEXT("Popularity"), hypeLevels);
    for (const auto& level : hypeLevels)
    {
      // we check our new level
      if (m_currentPopularity >= level->minRequiredHype && m_currentPopularity <= level->maxRequiredHype)
      {
        newLevel = level->level;
        minValue = level->minRequiredHype;
        maxValue = level->maxRequiredHype;
        break;
      }
    }

    //@remind to change the table, it makes no sense that each level has its own min and max, should it not be the total
    // value-duration?
    // if the new level is greater that the current level we reset the popularity value to the min limit
    if (newLevel > m_hypeLevel->level)
    {
      m_currentPopularity = minValue;
    }
    // if the new level is less that the current level we reset the popularity value to the max limit
    else if (newLevel < m_hypeLevel->level)
    {
      m_currentPopularity = maxValue;
    }
  }
}

float UPopularityComponent::GetMultiplier() const
{
  return m_hypeLevel->multiplier;
}
// EOF
