#include "HypeComponent.h"

UHypeComponent::UHypeComponent() :
	BaseHypeValue(10)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHypeComponent::AddHype(const int& _Points)
{
	BaseHypeValue += _Points;
}

int UHypeComponent::GetHype() const
{
	return BaseHypeValue;
}

void UHypeComponent::ResetHype()
{
	BaseHypeValue = 0;
}


