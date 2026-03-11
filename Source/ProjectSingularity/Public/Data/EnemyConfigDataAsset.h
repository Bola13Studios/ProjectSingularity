// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyConfigDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSINGULARITY_API UEnemyConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Enemy Speed"))
	float Maxspeed;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Enemy Damage"))
	float damage;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Enemy Health"))
	float health;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Enemy Max Health"))
	float maxHealth;
};
