// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Character/Enemy/ChildEnemies/ChaserEnemy.h"
#include "ProjectSingularity/Public/Data/ChaserEnemyDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"

void AChaserEnemy::BeginPlay()
{
  Super::BeginPlay();

	m_pChaserDataAsset = Cast<UChaserEnemyDataAsset>(m_pCharacterDataAsset);
  UCharacterMovementComponent* charMoveComp = GetCharacterMovement();
	if (IsValid(m_pChaserDataAsset))
	{
    m_timeStager = m_pChaserDataAsset->timeStager;
    m_timePause = m_pChaserDataAsset->timePause;
    m_attackRange = m_pChaserDataAsset->attackRange;
    m_distancetraveled = m_pChaserDataAsset->distancetraveled;

	}
 
}
