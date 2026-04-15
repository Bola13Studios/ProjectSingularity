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

float UHypeModifierComponent::GetModifierValue(FName _modifier) const
{
  for (const auto& modifier : m_hypeModifiers)
  {
    if (modifier->id == _modifier)
    {
      return modifier->value;
    }
  }
  return 0.0f;
}

bool UHypeModifierComponent::AddModifier(FName _modifier)
{
  // get the gameinstance first
  if (UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance()))
  {
    TArray<FHypeModifiers*> hypeModifiers;
    // check if the key name is in the data table
    FHypeModifiers* modifierRow =
        gameInstance->m_hypeModifiersDataTable->FindRow<FHypeModifiers>(_modifier, TEXT("Modifier"));

    if (!modifierRow)
    {
      UE_LOG(LogTemp, Error, TEXT("Unable to find the modifier. Are you sure the row name and identifier name match?"));
      return false;
    }

    // add the values to the array
    m_hypeModifiers.Add(modifierRow); // praying the lord this fucker doesn't get deleted here
    return true;
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
  bool removedAny = false;

  for (int i = m_hypeModifiers.Num() - 1; i >= 0; i--)
  {
    if (m_hypeModifiers[i]->isTemporary)
    {
      m_hypeModifiers.RemoveAt(i);
      removedAny = true;
    }
  }

  return removedAny;
}
