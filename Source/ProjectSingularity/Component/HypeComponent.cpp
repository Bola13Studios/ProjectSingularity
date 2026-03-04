#include "HypeComponent.h"

UHypeComponent::UHypeComponent() :
	Multiplier(1),
	Points(10)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHypeComponent::AddPoints(const int& _Points)
{
	Points += _Points;
}

int UHypeComponent::GetPoints() const
{
	return FMath::CeilToInt(Points * Multiplier);
}

void UHypeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHypeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

