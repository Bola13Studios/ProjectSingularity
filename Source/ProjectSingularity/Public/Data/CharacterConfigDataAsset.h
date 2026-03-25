// Fill out your copyright notice in the Description page of Project Settings.
/************************************************************************
 * @description: This class defines a character configuration data asset that can be used to store various properties
 *and settings related to characters in the game. The data asset includes properties such as maximum walk speed, health,
 *and maximum health. This allows for easy customization and management of character attributes without the need for
 *hardcoding values in the character classes.
 * @author: Lucia Fernandez
 * @date: 25/03/2026
 * @edited_by: 
 ************************************************************************/

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

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Health"))
	float health;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Max Health"))
	float maxHealth;
};
