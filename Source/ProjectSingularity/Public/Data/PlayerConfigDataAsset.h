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

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Jump Z Velocity"))
	float jumpZVelocity;

	//When falling, amount of lateral movement control available to the character. 0 = no control, 1 = full control at MaxWalkSpeed
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Air Control", ClampMin = "0", ClampMax = "1"))
	float airControl;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Gravity Scale"))
	float gravityScale;
};
