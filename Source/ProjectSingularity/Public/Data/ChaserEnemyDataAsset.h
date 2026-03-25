// Fill out your copyright notice in the Description page of Project Settings.
/************************************************************************
 * @description: This class defines a chaser enemy data asset that inherits from the UEnemyConfigDataAsset class. The
 *chaser enemy data asset
 * @author: Lucia Fernandez
 * @date: 25/02/2026
 * @edited_by: <co editor>
 *             <co editor>
 ************************************************************************/
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

	// Properties specific to the chaser enemy character
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Stager Timer"))
	float timeStager;
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Pause Time"))
	float timePause;
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Attack Range"))
	float attackRange;
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Distance Traveled"))
	float distancetraveled;

};
