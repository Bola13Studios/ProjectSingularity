// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/State Machine/States.h"
#include "Gameplay/Weapons/WeaponBase.h"
#include "Gameplay/Character/BaseCharacter.h"
#include "Gameplay/Character/Player/PlayerCharacter.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter* UCharacterBase::GetCharacter() const
{
  return GetOwnerAs<ABaseCharacter>();
}

AWeaponBase* UWeaponBaseState::GetWeapon() const
{
  return GetOwnerAs<AWeaponBase>();
}

#pragma region MovementStates

void UGroundMovementState::Init()
{
  //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Ground Movement State"));
  CanUpdateTick = true;
}

void UGroundMovementState::Update(float _DeltaTime)
{
  Super::Update(_DeltaTime);
  APlayerCharacter* PlayerOwner = GetOwnerAs<APlayerCharacter>();
  if (PlayerOwner && (PlayerOwner->GetVelocity().Z < 0. && !PlayerOwner->IsGrounded()))
  {
    PlayerOwner->RequestChangeState(UFallingState::StaticClass());
  }
}

void UGroundMovementState::HandleInput(const FInputActionValue& _inputValue)
{
  if (APlayerCharacter* PlayerOwner = GetOwnerAs<APlayerCharacter>())
  {
    PlayerOwner->MoveInternal(_inputValue.Get<FVector2D>());
  }
}

void UGroundMovementState::Exit()
{
}

//------------UDashingState

void UDashingState::Init()
{
  if (APlayerCharacter* PlayerOwner = GetOwnerAs<APlayerCharacter>())
  {
    PlayerOwner->Dash();
  }
  //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Dashing State"));
}

void UDashingState::Update(float _DeltaTime)
{
}

void UDashingState::Exit()
{
  if (APlayerCharacter* PlayerOwner = GetOwnerAs<APlayerCharacter>())
  {
    PlayerOwner->StopDash();
  }
}

//-----------UJumpingState
void UJumpingState::Init()
{
  //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Jumping State"));
  if (APlayerCharacter* PlayerOwner = GetOwnerAs<APlayerCharacter>())
  {
    PlayerOwner->Jump();
  }
  CanUpdateTick = true;
}

void UJumpingState::Update(float _DeltaTime)
{
  Super::Update(_DeltaTime);
  if (APlayerCharacter* PlayerOwner = GetOwnerAs<APlayerCharacter>())
  {
    if (PlayerOwner->GetVelocity().Z <= 0.)
    {
      PlayerOwner->RequestChangeState(UFallingState::StaticClass());
    }
  }
}

void UJumpingState::HandleInput(const FInputActionValue& _inputValue)
{
  if (APlayerCharacter* PlayerOwner = GetOwnerAs<APlayerCharacter>())
  {
    PlayerOwner->MoveInternal(_inputValue.Get<FVector2D>());
  }
}

void UJumpingState::Exit()
{
  if (APlayerCharacter* PlayerOwner = GetOwnerAs<APlayerCharacter>())
  {
    PlayerOwner->StopJumping();
  }
}

//-----------UFallingState

void UFallingState::Init()
{
  //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Falling State"));
  CanUpdateTick = true;
}

void UFallingState::Update(float _DeltaTime)
{
  APlayerCharacter* PlayerOwner = GetOwnerAs<APlayerCharacter>();
  if (PlayerOwner && (PlayerOwner->IsGrounded() && PlayerOwner->GetVelocity().Z <= 0.))
  {
    PlayerOwner->RequestChangeState(UGroundMovementState::StaticClass());
  }
}

void UFallingState::HandleInput(const FInputActionValue& _inputValue)
{
  if (APlayerCharacter* PlayerOwner = GetOwnerAs<APlayerCharacter>())
  {
    PlayerOwner->MoveInternal(_inputValue.Get<FVector2D>());
  }
}

void UFallingState::Exit()
{
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
