// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/BaseCharacter.h"
#include "BaseEnemy.generated.h"


class UHealthComponent;
class UEnemyConfigDataAsset;
/**
 * 
 */
UCLASS()
class PROJECTSINGULARITY_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:

	ABaseEnemy();



protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Stats|Enemy", meta = (DisplayName = "Enemy Damage"))
	float m_damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13|Components")
	TObjectPtr<UHealthComponent> m_healthComponent;

private:

	UPROPERTY(EditAnywhere, Category = "Data Asset|Enemy", meta = (DisplayName = "Enemy Config Data Asset"))
	TObjectPtr<UEnemyConfigDataAsset> m_EnemyDataAsset;
	
};
