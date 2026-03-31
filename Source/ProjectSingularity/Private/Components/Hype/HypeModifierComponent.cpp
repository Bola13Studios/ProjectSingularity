#include "Components/Hype/HypeModifierComponent.h"
#include "Components/Hype/HypeModifiers.h"

UHypeModifierComponent::UHypeModifierComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
}

void UHypeModifierComponent::BeginPlay()
{
  Super::BeginPlay();
}

void UHypeModifierComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TArray<int> UHypeModifierComponent::GetActiveModifiers() const
{
  return TArray<int>();
}

bool UHypeModifierComponent::AddModifier(FName _modifier)
{
  return false;
}

bool UHypeModifierComponent::RemoveModifier(FName _modifier)
{
  return false;
}

float UHypeModifierComponent::GetTotalModifiers() const
{
  return 0.0f;
}

bool UHypeModifierComponent::ClearModifiers() const
{
  return false;
}
