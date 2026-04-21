// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Animation/BaseAnimInstance.h"
#include "Gameplay/Character/Player/PlayerCharacter.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class PROJECTSINGULARITY_API UPlayerAnimInstance : public UBaseAnimInstance
{
  GENERATED_BODY()

private:
  UPROPERTY()
  TObjectPtr<APlayerCharacter> m_player;

  /** Check if Player is on falling state */
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "IsFalling",
            meta = (AllowPrivateAccess = "true"))
  bool m_isFalling;

  /** Check if Player is on falling state */
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "IsJumping",
            meta = (AllowPrivateAccess = "true"))
  bool m_isJumping;

  /** Check if Player should move when he's on the idle state */
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "ShouldMove",
            meta = (AllowPrivateAccess = "true"))
  bool m_shouldMove;

public:
  virtual void NativeInitializeAnimation() override;
  virtual void NativeUpdateAnimation(float DeltaSeconds) override;

  /** Function to manage the should move boolean variable */
  UFUNCTION(BlueprintCallable, Category = "Animation Flow")
  void UpdateShouldMove();

  /** Function to manage the is falling boolean variable */
  UFUNCTION(BlueprintCallable, Category = "Animation Flow")
  void SetIsFalling(bool isFalling);

  /** Function to manage the is jumping boolean variable */
  UFUNCTION(BlueprintCallable, Category = "Animation Flow")
  void SetIsJumping(bool isJumping);
};
