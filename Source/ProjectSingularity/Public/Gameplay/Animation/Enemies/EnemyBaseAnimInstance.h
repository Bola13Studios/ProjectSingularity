// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Animation/BaseAnimInstance.h"
#include "EnemyBaseAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class PROJECTSINGULARITY_API UEnemyBaseAnimInstance : public UBaseAnimInstance
{
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable)
  void PlayAnimation(FName name);

public:
  UPROPERTY(EditAnywhere)
  TObjectPtr<UAnimMontage> enemyAnimMontage;
};
