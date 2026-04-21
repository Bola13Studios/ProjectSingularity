// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Animation/Player/PlayerAnimInstance.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
  Super::NativeInitializeAnimation();

  m_player = Cast<APlayerCharacter>(GetBaseCharacter());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
  Super::NativeUpdateAnimation(DeltaSeconds);

  UpdateShouldMove();
}

void UPlayerAnimInstance::UpdateShouldMove()
{
  m_shouldMove = (m_groundSpeed > 5.f) && (!m_isFalling);
}

void UPlayerAnimInstance::SetIsFalling(bool isFalling)
{
  m_isFalling = isFalling;
}

void UPlayerAnimInstance::SetIsJumping(bool isJumping)
{
  m_isJumping = isJumping;
}
