// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyConfigDataAsset.h"
#include "ChaserEnemyDataAsset.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class PROJECTSINGULARITY_API UChaserEnemyDataAsset : public UEnemyConfigDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Stager Timer"))
	float timeStager;
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Pause Time"))
	float timePause;
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Attack Range"))
	float attackRange;
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Distance Traveled"))
	float distancetraveled;

};
