

#include "ProjectSingularity/Public/Components/HealthComponent.h"
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

void UHealthComponent::ApplyDamage(float Damage, AActor* InstigatorActor)
{
	if (Damage <= 0.0f || CurrentHealth <= 0.0f)
	{
		return;
	}

	const float Old = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	BroadcastChanged(Old, InstigatorActor);
}

void UHealthComponent::Heal(float Amount, AActor* InstigatorActor)
{
	if (Amount <= 0.0f || CurrentHealth >= MaxHealth)
	{
		return;
	}

	const float Old = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	BroadcastChanged(Old, InstigatorActor);
}

void UHealthComponent::BroadcastChanged(float OldHealth, AActor* InstigatorActor)
{
	const float Delta = CurrentHealth - OldHealth;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, Delta, InstigatorActor);
}