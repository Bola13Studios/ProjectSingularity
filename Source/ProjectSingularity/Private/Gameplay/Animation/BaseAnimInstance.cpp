// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Animation/BaseAnimInstance.h"
#include "Gameplay/Character/BaseCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>

void UBaseAnimInstance::NativeInitializeAnimation()
{
  Super::NativeInitializeAnimation();

  m_baseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());

  if (m_baseCharacter)
  {
    m_movementComponent = m_baseCharacter->GetMovementComponent();
  }
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
  Super::NativeUpdateAnimation(DeltaSeconds);

  if (m_baseCharacter != NULL && m_movementComponent != NULL)
  {
    UpdateGroundSpeed();
    UpdateAirSpeed();
    UpdateDirection();
    UpdateAimVertical();
  }
}

// Not working for enemies because their MaxSpeed changes depending on their state
void UBaseAnimInstance::UpdateGroundSpeed()
{
  m_groundSpeed = UKismetMathLibrary::VSizeXY(m_baseCharacter->GetVelocity());
  m_groundSpeed = (m_groundSpeed / m_movementComponent->GetMaxSpeed()) * 100.f;
}

void UBaseAnimInstance::UpdateAirSpeed()
{
  m_airSpeed = m_baseCharacter->GetVelocity().Z;
}

void UBaseAnimInstance::UpdateDirection()
{
  m_direction.X =
      FVector::DotProduct(m_baseCharacter->GetVelocity().GetSafeNormal(), m_baseCharacter->GetActorForwardVector())
      * m_groundSpeed;
  m_direction.Y =
      FVector::DotProduct(m_baseCharacter->GetVelocity().GetSafeNormal(), m_baseCharacter->GetActorRightVector())
      * m_groundSpeed;
}

void UBaseAnimInstance::UpdateAimVertical()
{
  m_aimVertical = FMath::ClampAngle(m_baseCharacter->GetControlRotation().Pitch, -90.f, 90.f) / 90.f * 100;
}

void UBaseAnimInstance::Fire()
{
  Montage_Play(fireMontage);
}

TObjectPtr<ABaseCharacter> UBaseAnimInstance::GetBaseCharacter()
{
  return m_baseCharacter;
}
