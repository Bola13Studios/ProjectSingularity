// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Character/Enemy/ChildEnemies/ChaserEnemy.h"
#include "ProjectSingularity/Public/Data/ChaserEnemyDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include <Systems/BaseGameInstance.h>

void AChaserEnemy::BeginPlay()
{
  Super::BeginPlay();

  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());

  UCharacterMovementComponent* charMoveComp = GetCharacterMovement();
	if (IsValid(m_pChaserDataAsset))
	{
    m_timeStager = pGameInstance->m_pChaserDataAsset->m_timeStager;
    m_timePause = pGameInstance->m_pChaserDataAsset->m_timePause;
    m_attackRange = pGameInstance->m_pChaserDataAsset->m_attackRange;
    m_dashForce = pGameInstance->m_pChaserDataAsset->m_dashForce;

	}
 
}
