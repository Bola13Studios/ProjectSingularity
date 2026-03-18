// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/CharacterConfigDataAsset.h"
#include "PlayerConfigDataAsset.generated.h"


class UCharacterConfigDataAsset;
/**
 * 
 */
UCLASS()
class PROJECTSINGULARITY_API UPlayerConfigDataAsset : public UCharacterConfigDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Max Walk Speed", ClampMin = "0"))
	float maxWalkSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Jump Z Velocity", ClampMin = "0"))
	float jumpZVelocity;

	//When falling, amount of lateral movement control available to the character. 0 = no control, 1 = full control at MaxWalkSpeed
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Air Control", ClampMin = "0", ClampMax = "1"))
	float airControl;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Gravity Scale", ClampMin = "0"))
	float gravityScale;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Ground Friction", ClampMin = "0"))
	float groundFriction;

	UPROPERTY(EditAnywhere, Category = "Movement|Dash", meta = (DisplayName = "Dash Distance", ClampMin = "0.1"))
	float dashDistance;

	UPROPERTY(EditAnywhere, Category = "Movement|Dash", meta = (DisplayName = "Dash Time", ClampMin = "0.1"))
	float dashTime;

	UPROPERTY(EditAnywhere, Category = "Movement|Dash", meta = (DisplayName = "Dash Cooldown", ClampMin = "0.1"))
	float dashCooldown;
};
