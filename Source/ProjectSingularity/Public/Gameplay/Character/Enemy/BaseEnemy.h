// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/BaseCharacter.h"
#include "BaseEnemy.generated.h"



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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Enemy Data", meta = (DisplayName = "Data Asset"))
	TObjectPtr<UEnemyConfigDataAsset> m_EnemyDataAsset;

	UPROPERTY(VisibleAnywhere, Category = "Stats|Enemy", meta = (DisplayName = "Enemy Damage"))
	float m_damage;

private:

	
};
