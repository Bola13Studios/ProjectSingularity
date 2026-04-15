#include "ProjectSingularity/Public/Components/HealthComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeReceiverComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeSourceComponent.h"
#include "ProjectSingularity/Public/Systems/GameManagerSubsystem.h"
#include "ProjectSingularity/Public/Utils/StatHelpers.h"
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

				// STATS
        UGameManagerSubsystem::AddStat(this, STAT_PATH(hype.total_gained_hype_normal_hit),
                                       (int32)sourceHype->GetHype());
        UGameManagerSubsystem::AddStat(this, STAT_PATH(enemy.chaser.enemy_total_deaths), 1);
        if (hasHitBeenCritical)
        {
          UGameManagerSubsystem::AddStat(this, STAT_PATH(hype.total_gained_hype_critical_hit),
                                         (int32)sourceHype->GetHype());
          UGameManagerSubsystem::AddStat(this, STAT_PATH(enemy.chaser.enemy_total_critical_deaths), 1);
        }
			}
		}

		OnDeath.Broadcast(InstigatorActor);
	}
	BroadcastChanged(Old, InstigatorActor);

	// HEALTH STATS
	// only the player will have this component
	if (GetOwner()->FindComponentByClass<UHypeReceiverComponent>())
  {
    UGameManagerSubsystem::AddStat(this, STAT_PATH(enemy.chaser.enemy_total_hits), 1);
		if (_Amount < 0.0f)
		{
			UGameManagerSubsystem::AddStat(this, STAT_PATH(combat.health.combat_total_lost_health), (int32)_Amount);
		}
		else
		{
			UGameManagerSubsystem::AddStat(this, STAT_PATH(combat.health.combat_total_gained_health), (int32)_Amount);
		}
	}
}

void UHealthComponent::BroadcastChanged(float OldHealth, AActor* InstigatorActor) const
{
	const float Delta = CurrentHealth - OldHealth;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, Delta, InstigatorActor);
}