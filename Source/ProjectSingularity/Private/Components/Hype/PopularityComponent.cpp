#include "ProjectSingularity/Public/Components/Hype/PopularityComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeLevels.h"
#include "ProjectSingularity/Public/Systems/BaseGameInstance.h"
#include <Engine/World.h>

UPopularityComponent::UPopularityComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
  m_currentPopularity = 0.0f;
  multiplier = 0.0f;
}

float UPopularityComponent::GetNormalizedPopularity() const
{
  float range = m_nextLevelMin - m_currentLevelMin;

  if (range <= 0.f) return 0.f;

  float normalizedValue = (m_currentPopularity - m_currentLevelMin) / range;

  return FMath::Clamp(normalizedValue, 0.0f, 1.0f);
}

void UPopularityComponent::BeginPlay()
{
  Super::BeginPlay();
  if (UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance()))
  {
    if (!IsValid(gameInstance->m_hypePopularityDataTable))
    {
      UE_LOG(
          LogTemp, Error,
          TEXT("Unable to retreive the PopularityDataTable from the BaseGameInstance. Are you sure it was assigned?"));
      return;
    }

    // we get the first level as default
    TArray<FHypeLevels*> hypeLevels;
    gameInstance->m_hypePopularityDataTable->GetAllRows(TEXT("Popularity"), hypeLevels);
    // saving the values of the current level, so that we can use them for calculations and comparisons later on
    level = hypeLevels[0]->level;
    multiplier = hypeLevels[0]->multiplier;
    m_decayRate = hypeLevels[0]->decayRate;
  }

  GetWorld()->GetTimerManager().SetTimer(m_decayTimerHandle, this, &UPopularityComponent::HandleDecay, m_decayInterval,
                                         true);
}

void UPopularityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  GetWorld()->GetTimerManager().ClearTimer(m_decayTimerHandle);
  Super::EndPlay(EndPlayReason);
}

void UPopularityComponent::HandleDecay()
{
  if (m_currentPopularity <= 0) return;

  float decayAmount = CalculateDecay();

  m_currentPopularity = FMath::Clamp(m_currentPopularity - decayAmount, 0, m_maxPopularity);

  UpdateLevel();
}

float UPopularityComponent::CalculateDecay() const
{
  float decay_ = m_decayRate;

  // we can apply the level multiplier to the decay rate, so that the higher the level, the faster the decay
  decay_ *= (1.f + level * 0.2f);

  return decay_;
}

float UPopularityComponent::GetCurrentPopularity() const
{
  return m_currentPopularity;
}

int UPopularityComponent::GetPopularityLevel() const
{
  return level;
}

void UPopularityComponent::IncreasePopularity(float _value)
{
  m_currentPopularity = FMath::Clamp(m_currentPopularity + _value, 0.f, m_maxPopularity);

  // we then check if we surpassed the current level max limit
  UpdateLevel();

  // and reset the decay timer so that it starts counting from the moment we generated hype
  GetWorld()->GetTimerManager().ClearTimer(m_decayTimerHandle);
  GetWorld()->GetTimerManager().SetTimer(m_decayTimerHandle, this, &UPopularityComponent::HandleDecay, m_decayInterval,
                                         true);
}

void UPopularityComponent::DecreasePopularity(float _value)
{
  m_currentPopularity = FMath::Clamp(m_currentPopularity - _value, 0.f, m_maxPopularity);

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

    gameInstance->m_hypePopularityDataTable->GetAllRows(TEXT("Popularity"), hypeLevels);
    // we loop through all the levels to find the one that corresponds to our current popularity
    for (int i = 0; i < hypeLevels.Num(); i++)
    {
      if (m_currentPopularity >= hypeLevels[i]->requiredValue)
      {
        // saving the values of the current level, so that we can use them for calculations and comparisons later on
        level = hypeLevels[i]->level;
        multiplier = hypeLevels[i]->multiplier;
        m_decayRate = hypeLevels[i]->decayRate;

        m_currentLevelMin = hypeLevels[i]->requiredValue;

        // we also get the next level min value, so that we can use it for comparisons later on. If there is no next
        // level, we set it to the current level min + 100 as a default value
        if (hypeLevels.IsValidIndex(i + 1))
        {
          m_nextLevelMin = hypeLevels[i + 1]->requiredValue;
        }
        else
        {
          m_nextLevelMin = m_currentLevelMin + 100.f;
        }
      }
    }
  }
  // broadcasting that the popularity has changed so that the UI can update accordingly
  onPopularityChanged.Broadcast();
}

float UPopularityComponent::GetMultiplier() const
{
  return multiplier;
}
