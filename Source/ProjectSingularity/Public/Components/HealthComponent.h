
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, float, Current, float, Max, float, Delta, AActor*, InstigatorActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PROJECTSINGULARITY_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetHealthPercent() const
	{
		return (MaxHealth > 0.0f) ? (CurrentHealth / MaxHealth) : 0.0f;
	}

	UFUNCTION(BlueprintCallable, Category="Health")
	void ApplyDamage(float Damage, AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category="Health")
	void Heal(float Amount, AActor* InstigatorActor);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Health")
	float CurrentHealth = 100.0f;

private:
	void BroadcastChanged(float OldHealth, AActor* InstigatorActor);
};