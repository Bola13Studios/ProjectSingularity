// Fill out your copyright notice in the Description page of Project Settings.
/************************************************************************
 * @description: This class defines a chaser enemy character that inherits from the ABaseEnemy class. The chaser enemy
 *is a specific type of enemy character that has unique properties and behaviors. The class includes properties such as
 *stager timer, pause time, attack range, and distance traveled, which can be used to control the movement and attack
 *patterns of the chaser enemy. The class also includes a reference to a chaser enemy data asset for storing
 *chaser-specific data.
 * @author: Lucia Fernandez
 * @date: 25/02/2026
 * @edited_by: 
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/Enemy/BaseEnemy.h"
#include "ChaserEnemy.generated.h"

/**
 * 
 */
class UChaserEnemyDataAsset;

UCLASS()
class PROJECTSINGULARITY_API AChaserEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;
	
	// Properties specific to the chaser enemy character
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Stager Timer"))
	float m_timeStager;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Pause Time"))
	float m_timePause;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Attack Range"))
	float m_attackRange;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Distance Traveled"))
	float m_distancetraveled;

	private:
  // Reference to a chaser enemy data asset for storing chaser-specific data
  TObjectPtr<UChaserEnemyDataAsset> m_pChaserDataAsset;
};
