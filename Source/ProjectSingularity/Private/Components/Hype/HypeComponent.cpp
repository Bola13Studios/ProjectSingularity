#include "ProjectSingularity/Public/Components/Hype/HypeComponent.h"

UHypeComponent::UHypeComponent() :
	BaseHypeValue(10)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHypeComponent::AddHype(const int& _Points)
{
	BaseHypeValue += _Points;
	OnHypeChanged.Broadcast(BaseHypeValue, _Points);
}

int UHypeComponent::GetHype() const
{
	return BaseHypeValue;
}

void UHypeComponent::ResetHype()
{
	const int OldValue = BaseHypeValue;
	BaseHypeValue = 0;

	OnHypeChanged.Broadcast(BaseHypeValue, -OldValue);
}


