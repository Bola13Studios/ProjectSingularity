#include "ProjectSingularity/Public/Components/HealthComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeReceiverComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeSourceComponent.h"
#include "GameFramework/Actor.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, 0.0f, GetOwner());
}

void UHealthComponent::ChangeHealth(float _Amount, AActor* InstigatorActor)
{
	if ((_Amount > 0.f && CurrentHealth >= MaxHealth) || (_Amount < 0.f && CurrentHealth <= 0.f))
	{
		return;
	}

	const float Old = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + _Amount, 0.0f, MaxHealth);
	if (_Amount < 0.f && FMath::IsNearlyZero(CurrentHealth))
	{
		if (UHypeReceiverComponent* receiver = InstigatorActor->FindComponentByClass<UHypeReceiverComponent>())
		{
			if (UHypeSourceComponent* sourceHype = GetOwner()->FindComponentByClass<UHypeSourceComponent>())
			{
        receiver->RegisterKill(sourceHype, hasHitBeenCritical);
			}
		}

		OnDeath.Broadcast(InstigatorActor);
	}
	BroadcastChanged(Old, InstigatorActor);
}

void UHealthComponent::BroadcastChanged(float OldHealth, AActor* InstigatorActor) const
{
	const float Delta = CurrentHealth - OldHealth;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, Delta, InstigatorActor);
}