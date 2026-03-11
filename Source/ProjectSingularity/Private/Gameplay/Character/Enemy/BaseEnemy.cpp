// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Character/Enemy/BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectSingularity/Public/Data/EnemyConfigDataAsset.h"
#include "ProjectSingularity/Public/Components/HealthComponent.h"

ABaseEnemy::ABaseEnemy()
{
  m_healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));


}

void ABaseEnemy::BeginPlay()
{
	UCharacterMovementComponent* charMoveComp = GetCharacterMovement();
	if (IsValid(m_EnemyDataAsset) && IsValid(charMoveComp))
	{
		charMoveComp->MaxWalkSpeed = m_EnemyDataAsset->Maxspeed;
		m_healthComponent->SetHealth(m_EnemyDataAsset->health);
		m_healthComponent->SetMaxHealth(m_EnemyDataAsset->maxHealth);
		m_damage = m_EnemyDataAsset->damage;


	}
}
