// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyConfigDataAsset.h"
#include "ChaserEnemyDataAsset.generated.h"

/**
 * 
 */
class UEnemyConfigDataAsset;

UCLASS()
class PROJECTSINGULARITY_API UChaserEnemyDataAsset : public UEnemyConfigDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Stager Timer"))
	float timeStager;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Pause Time"))
	float timePause;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Attack Range"))
	float attackRange;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Distance Traveled"))
	float distancetraveled;

};
