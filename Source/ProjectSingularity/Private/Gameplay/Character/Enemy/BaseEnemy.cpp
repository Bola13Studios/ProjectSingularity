// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Character/Enemy/BaseEnemy.h"
#include "ProjectSingularity/Public/Data/EnemyConfigDataAsset.h"


ABaseEnemy::ABaseEnemy()
{
  m_healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));


}

void ABaseEnemy::BeginPlay()
{
	m_EnemyDataAsset = Cast<UEnemyConfigDataAsset>(m_pCharacterDataAsset);
	if (IsValid(m_EnemyDataAsset))
	{
		m_damage = m_EnemyDataAsset->damage;
	}
}
