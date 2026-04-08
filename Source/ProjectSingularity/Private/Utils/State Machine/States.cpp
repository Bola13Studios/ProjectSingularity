// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/State Machine/States.h"
#include "Gameplay/Weapons/WeaponBase.h"
#include "Gameplay/Character/BaseCharacter.h"
#include "Gameplay/Character/Player/PlayerCharacter.h"
#include "Gameplay/Animation/Player/PlayerAnimInstance.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter* UCharacterBase::GetCharacter() const
{
  return GetOwnerAs<APlayerCharacter>();
}

AWeaponBase* UWeaponBaseState::GetWeapon() const
{
  return GetOwnerAs<AWeaponBase>();
}

#pragma region MovementStates

void UGroundMovementState::Init()
{
  // GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Ground Movement State"));
  CanUpdateTick = true;
}

void UGroundMovementState::Update(float _DeltaTime)
{
  Super::Update(_DeltaTime);
  if ((GetCharacter()->GetVelocity().Z < 0. && !GetCharacter()->IsGrounded()))
  {
    GetCharacter()->RequestChangeState(UFallingState::StaticClass());
  }
}

void UGroundMovementState::HandleInput(const FInputActionValue& _inputValue)
{
  GetCharacter()->MoveInternal(_inputValue.Get<FVector2D>());
}

void UGroundMovementState::Exit()
{
}

//------------UDashingState

void UDashingState::Init()
{
  GetCharacter()->Dash();
  // GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Dashing State"));
}

void UDashingState::Update(float _DeltaTime)
{
}

void UDashingState::Exit()
{
  GetCharacter()->StopDash();
}

//-----------UJumpingState
void UJumpingState::Init()
{
  // GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Jumping State"));
  GetCharacter()->Jump();
  GetCharacter()->GetPlayerAnimInstance()->SetIsJumping(true);
  CanUpdateTick = true;
}

void UJumpingState::Update(float _DeltaTime)
{
  Super::Update(_DeltaTime);
  if (GetCharacter()->GetVelocity().Z <= 0.)
  {
    GetCharacter()->RequestChangeState(UFallingState::StaticClass());
  }
}

void UJumpingState::HandleInput(const FInputActionValue& _inputValue)
{
  GetCharacter()->MoveInternal(_inputValue.Get<FVector2D>());
}

void UJumpingState::Exit()
{
  GetCharacter()->StopJumping();
  GetCharacter()->GetPlayerAnimInstance()->SetIsJumping(false);
}

//-----------UFallingState

void UFallingState::Init()
{
  // GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Falling State"));
  CanUpdateTick = true;
  GetCharacter()->GetPlayerAnimInstance()->SetIsFalling(true);
}

void UFallingState::Update(float _DeltaTime)
{
  if (GetCharacter()->IsGrounded() && GetCharacter()->GetVelocity().Z <= 0.)
  {
    GetCharacter()->RequestChangeState(UGroundMovementState::StaticClass());
  }
}

void UFallingState::HandleInput(const FInputActionValue& _inputValue)
{
  GetCharacter()->MoveInternal(_inputValue.Get<FVector2D>());
}

void UFallingState::Exit()
{
  GetCharacter()->GetPlayerAnimInstance()->SetIsFalling(false);
}

#pragma endregion

#pragma region WeaponStates

//-------------UWeaponReload-------------
void UWeaponReload::Init()
{
  GetWeapon()->Reload();
}

void UWeaponReload::Update(float _DeltaTime)
{
}

void UWeaponReload::Exit()
{
}

//-------------UWeaponChangeMode-------------
void UWeaponChangeMode::Init()
{
  GetWeapon()->ChangeMode();
}

void UWeaponChangeMode::Update(float _DeltaTime)
{
}

void UWeaponChangeMode::Exit()
{
}
#pragma endregion
