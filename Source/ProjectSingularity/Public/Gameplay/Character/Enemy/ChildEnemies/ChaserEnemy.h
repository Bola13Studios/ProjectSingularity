// Fill out your copyright notice in the Description page of Project Settings.

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
	

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Stager Timer"))
	float m_timeStager;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Pause Time"))
	float m_timePause;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Attack Range"))
	float m_attackRange;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Distance Traveled"))
	float m_distancetraveled;

	private:
  TObjectPtr<UChaserEnemyDataAsset> m_pChaserDataAsset;
};
