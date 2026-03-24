// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterConfigDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTSINGULARITY_API UCharacterConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Max Walk Speed"))
	float maxWalkSpeed;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Enemy Health"))
	float health;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Enemy Max Health"))
	float maxHealth;
};
