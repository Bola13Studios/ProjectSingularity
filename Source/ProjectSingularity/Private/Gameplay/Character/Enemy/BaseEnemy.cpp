#include "ProjectSingularity/Public/Gameplay/Character/Enemy/BaseEnemy.h"
#include "ProjectSingularity/Public/Data/EnemyConfigDataAsset.h"
#include "ProjectSingularity/Public/Components/Hype/HypeSourceComponent.h"
#include "ProjectSingularity/Public/Components/HealthComponent.h"

ABaseEnemy::ABaseEnemy()
{
  m_healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ABaseEnemy::BeginPlay()
{
  Super::BeginPlay();
  m_EnemyDataAsset = Cast<UEnemyConfigDataAsset>(m_pCharacterDataAsset);
  if (IsValid(m_EnemyDataAsset))
  {
    m_damage = m_EnemyDataAsset->damage;
  }
}

