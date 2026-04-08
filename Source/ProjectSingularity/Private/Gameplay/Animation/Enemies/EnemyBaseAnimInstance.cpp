// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Animation/Enemies/EnemyBaseAnimInstance.h"

void UEnemyBaseAnimInstance::PlayAnimation(FName name)
{
  Montage_Play(enemyAnimMontage);
  Montage_JumpToSection(name, enemyAnimMontage);
}
