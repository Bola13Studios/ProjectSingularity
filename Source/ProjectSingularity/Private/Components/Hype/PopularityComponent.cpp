#include "Components/Hype/PopularityComponent.h"
#include "Components/Hype/HypeLevels.h"
#include "Systems/BaseGameInstance.h"
#include <Engine/World.h>

UPopularityComponent::UPopularityComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
  m_currentPopularity = 0.0f;
  multiplier = 0.0f;
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

    for (const auto& row : hypeLevels)
    {
      if (!row) continue;

      if (m_currentPopularity >= row->requiredValue)
      {
        level = row->level;
        multiplier = row->multiplier;
        m_decayRate = row->decayRate;
      }
    }
  }
}

float UPopularityComponent::GetMultiplier() const
{
  return multiplier;
}
// EOF
