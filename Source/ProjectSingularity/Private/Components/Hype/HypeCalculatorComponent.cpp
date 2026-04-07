#include "Components/Hype/HypeCalculatorComponent.h"
#include "Components/Hype/HypeModifierComponent.h"
#include "Components/Hype/PopularityComponent.h"

UHypeCalculatorComponent::UHypeCalculatorComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
}

void UHypeCalculatorComponent::BeginPlay()
{
  Super::BeginPlay();
}

void UHypeCalculatorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UHypeCalculatorComponent::ApplyModifiers(UHypeModifierComponent* _modifier)
{
  return _modifier->GetTotalModifiers();
}

float UHypeCalculatorComponent::ApplyPopularity(UPopularityComponent* _popularity)
{
  return _popularity->GetMultiplier();
}

int UHypeCalculatorComponent::ComputeFinalHype(float& _baseHype, float& _modifiers, float& _popularity)
{
  return _baseHype + _baseHype * (_modifiers + _popularity);
}
