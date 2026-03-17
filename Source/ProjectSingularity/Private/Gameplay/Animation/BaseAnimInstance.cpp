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
        UpdateShouldMove();
        UpdateIsFalling();
        UpdateIsJumping();
        UpdateDirection();
        UpdateAimVertical();
    }
}

void UBaseAnimInstance::UpdateGroundSpeed()
{
    m_GroundSpeed = UKismetMathLibrary::VSizeXY(m_baseCharacter->GetVelocity());
    m_GroundSpeed = (m_GroundSpeed / m_movementComponent->GetMaxSpeed()) * 100.f;
}

void UBaseAnimInstance::UpdateAirSpeed()
{
    m_AirSpeed = m_baseCharacter->GetVelocity().Z;
}

void UBaseAnimInstance::UpdateShouldMove()
{
    m_ShouldMove = (m_GroundSpeed > 5.f) && (!m_IsFalling);
}

void UBaseAnimInstance::UpdateIsFalling()
{
    m_IsFalling = m_movementComponent->IsFalling();
}

void UBaseAnimInstance::UpdateIsJumping()
{
    //Need state machine - this is temporal
    m_IsJumping = m_baseCharacter->GetVelocity().Z > 0;
}

void UBaseAnimInstance::UpdateDirection()
{
    m_Direction.X = FVector::DotProduct(m_baseCharacter->GetVelocity().GetSafeNormal(), m_baseCharacter->GetActorForwardVector()) * m_GroundSpeed;
    m_Direction.Y = FVector::DotProduct(m_baseCharacter->GetVelocity().GetSafeNormal(), m_baseCharacter->GetActorRightVector()) * m_GroundSpeed;
}

void UBaseAnimInstance::UpdateAimVertical()
{
    m_aimVertical = FMath::ClampAngle(m_baseCharacter->GetControlRotation().Pitch, -90.f, 90.f) / 90.f * 100;
}

void UBaseAnimInstance::Fire()
{
    Montage_Play(fireMontage);
}
