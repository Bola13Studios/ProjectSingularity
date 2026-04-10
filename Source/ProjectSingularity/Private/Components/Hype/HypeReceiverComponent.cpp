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

  // adding modifiers
  if (_critical) m_modifierComponent->AddModifier("Critical");

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
  m_popularityComponent->IncreasePopularity(_source->popularityValue);

  // clearing modifiers
  m_modifierComponent->ClearModifiers();

  UE_LOG(LogTemp, Warning, TEXT("Kill → Base:%.2f Mod:%.2f Pop:%.2f Final:%.2f"), baseHype, totalModifier,
         popularityMultiplier, finalHype);
}

void UHypeReceiverComponent::RegisterMultiKill(int32 _killCount)
{
  if (_killCount > 1)
  {
    m_modifierComponent->AddModifier("MultiKill");
  }

  UE_LOG(LogTemp, Warning, TEXT("Multikill REAL: %d"), _killCount);
}

void UHypeReceiverComponent::RegisterStrikeKill()
{ /// REMOVE
  m_killStrikeCount++;

  // we reset the timer
  GetWorld()->GetTimerManager().ClearTimer(m_killStrikeTimer);

  GetWorld()->GetTimerManager().SetTimer(m_killStrikeTimer, this, &UHypeReceiverComponent::ResetStrikeKill, m_killStrikeWindow, false);

  UE_LOG(LogTemp, Warning, TEXT("Multikill count: %d"), m_killStrikeCount);
}


void UHypeReceiverComponent::ResetStrikeKill()
{
  m_killStrikeCount = 0;
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

void UHypeReceiverComponent::AddExternalModifier(const FName _modifier)
{
  if (m_modifierComponent) m_modifierComponent->AddModifier(_modifier);
}
