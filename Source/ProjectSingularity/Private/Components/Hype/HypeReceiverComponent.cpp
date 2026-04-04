#include "ProjectSingularity/Public/Components/Hype/HypeReceiverComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeSourceComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeCalculatorComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeModifierComponent.h"
#include "ProjectSingularity/Public/Components/Hype/PopularityComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeMultipliers.h"
#include "ProjectSingularity/Public/Components/Hype/HypeLevels.h"
#include "ProjectSingularity/Public/Gameplay/Character/Player/PlayerCharacter.h"

void UHypeReceiverComponent::BeginPlay()
{
  Super::BeginPlay();

  m_calculatorComponent = GetOwner()->FindComponentByClass<UHypeCalculatorComponent>();
  if (!m_calculatorComponent)
  {
    UE_LOG(LogTemp, Error,
           TEXT("Unable to retreive the HypeCalculatorComponent from the owner. Are you sure it was added?"));
  }

  m_modifierComponent = GetOwner()->FindComponentByClass<UHypeModifierComponent>();
  if (!m_modifierComponent)
  {
    UE_LOG(LogTemp, Error,
           TEXT("Unable to retreive the HypeModifierComponent from the owner. Are you sure it was added?"));
  }

  m_popularityComponent = GetOwner()->FindComponentByClass<UPopularityComponent>();
  if (!m_popularityComponent)
  {
    UE_LOG(LogTemp, Error,
           TEXT("Unable to retreive the PopularityComponent from the owner. Are you sure it was added?"));
  }
}

void UHypeReceiverComponent::RegisterKill(UHypeSourceComponent* _source, const bool& _critical)
{
  if (!_source || !m_calculatorComponent || !m_modifierComponent || !m_popularityComponent)
  {
    UE_LOG(LogTemp, Error, TEXT("Hype system not properly initialized"));
    return;
  }

  // incrementing the kill streak
  m_currentKillStreak++;

  RegisterMultiKill();
  if (m_multiKillCount > 1) m_modifierComponent->AddModifier("MultiKill");


  // adding modifiers
  if (_critical) m_modifierComponent->AddModifier("Critical");
  if (_source->m_isWeakPoint) m_modifierComponent->AddModifier("WeakPoint");

  // airborne
  APlayerCharacter* player = Cast<APlayerCharacter>(GetOwner());
  if (player && !player->IsGrounded()) m_modifierComponent->AddModifier("Airborne");

  // base hype
  float baseHype = _source->GetHype();

  // getting the total modifier from component
  float totalModifier = m_calculatorComponent->ApplyModifiers(m_modifierComponent);

  // getting the popularity multiplier from component
  float popularityMultiplier = m_calculatorComponent->ApplyPopularity(m_popularityComponent);

  // getting the final hype to add
  float finalHype = m_calculatorComponent->ComputeFinalHype(baseHype, totalModifier, popularityMultiplier);

  // adding final hype to the current value
  AddHype(finalHype);

  // updating the popularity index @remind > this is temporary will be changed after
  m_popularityComponent->IncreasePopularity(100.f); // tweakable

  // clearing modifiers
  m_modifierComponent->ClearModifiers();

  UE_LOG(LogTemp, Warning, TEXT("Kill → Base:%.2f Mod:%.2f Pop:%.2f Final:%.2f"), baseHype, totalModifier,
         popularityMultiplier, finalHype);
}

void UHypeReceiverComponent::RegisterMultiKill()
{
  m_multiKillCount++;

  // we reset the timer
  GetWorld()->GetTimerManager().ClearTimer(m_multiKillTimer);

  GetWorld()->GetTimerManager().SetTimer(m_multiKillTimer, this, &UHypeReceiverComponent::ResetMultiKill, m_multiKillWindow, false);

  UE_LOG(LogTemp, Warning, TEXT("Multikill count: %d"), m_multiKillCount);
}


void UHypeReceiverComponent::ResetMultiKill()
{
  m_multiKillCount = 0;
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
      if (m_currentHypeValue >= level->requiredValue)
      {
        m_currentHypeLevel = level->level;
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
