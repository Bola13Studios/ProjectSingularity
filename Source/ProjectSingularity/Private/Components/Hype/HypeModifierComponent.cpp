#include "Components/Hype/HypeModifierComponent.h"
#include "Components/Hype/HypeModifiers.h"
#include "Systems/BaseGameInstance.h"

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

TArray<FHypeModifiers*> UHypeModifierComponent::GetActiveModifiers() const
{
  return m_hypeModifiers;
}

bool UHypeModifierComponent::AddModifier(FName _modifier)
{
  // get the gameinstance first
  if (UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance()))
  {
    // check if the key name is in the data table

    // add the values to the array
  }

  return false;
}

bool UHypeModifierComponent::RemoveModifier(FName _modifier)
{
  // check if the key name is in the array
  for (int i = 0; i < m_hypeModifiers.Num(); i++)
  {
    if (_modifier == m_hypeModifiers[i]->id)
    {
      // safely delete the value from the array
      m_hypeModifiers.RemoveAt(i);
      return true;
    }
  }
  // we did not find the key :(. what where you looking for?
  return false;
}

float UHypeModifierComponent::GetTotalModifiers() const
{
  // calculate the overall value of all the saved modifiers
  float totalModifiers = 0.0f;
  for (auto& modifier : m_hypeModifiers)
  {
    totalModifiers += modifier->value;
  }
  return totalModifiers;
}

bool UHypeModifierComponent::ClearModifiers()
{
  // remove from the array only the modifiers that are set as temporary
  for (auto& modifier : m_hypeModifiers)
  {
    if (modifier->isTemporary)
    {
      return RemoveModifier(modifier->id);
    }
  }
  //@remind MISSING timer logic that removes modifiers based on their duration
  return false;
}
