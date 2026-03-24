// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/CharacterConfigDataAsset.h"
#include "EnemyConfigDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTSINGULARITY_API UEnemyConfigDataAsset : public UCharacterConfigDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere,BlueprintReadOnly, meta = (DisplayName = "Enemy Damage"))
	float damage;


};
