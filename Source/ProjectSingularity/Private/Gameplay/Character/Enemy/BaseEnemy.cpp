#include "ProjectSingularity/Public/Gameplay/Character/Enemy/BaseEnemy.h"
#include "ProjectSingularity/Public/Data/EnemyConfigDataAsset.h"
#include "ProjectSingularity/Public/Components/Hype/HypeSourceComponent.h"
#include "ProjectSingularity/Public/Components/HealthComponent.h"

ABaseEnemy::ABaseEnemy()
{
  m_healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
  m_healthComponent->OnDeath.AddUObject(this, &ABaseEnemy::KillMe);
}

void ABaseEnemy::KillMe(AActor* _instigator)
{
  OnEnemyDeath.Broadcast();
  Destroy();
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

void ABaseEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
  m_healthComponent->OnDeath.RemoveAll(this);
}

void ABaseEnemy::Attack(AActor* target)
{
}
