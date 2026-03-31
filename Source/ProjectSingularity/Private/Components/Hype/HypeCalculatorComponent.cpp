#include "Components/Hype/HypeCalculatorComponent.h"

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

int UHypeCalculatorComponent::CalculateBaseHype()
{
  return 0;
}

int UHypeCalculatorComponent::ApplyModifiers()
{
  return 0;
}

int UHypeCalculatorComponent::ApplyPopularity()
{
  return 0;
}

int UHypeCalculatorComponent::ComputeFinalHype()
{
  return 0;
}
