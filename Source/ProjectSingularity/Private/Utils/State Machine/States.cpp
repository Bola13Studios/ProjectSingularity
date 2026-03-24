// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/State Machine/States.h"
#include "Gameplay/Weapons/WeaponBase.h"
#include "Gameplay/Character/BaseCharacter.h"
#include "Gameplay/Character/Player/PlayerCharacter.h"
#include "InputActionValue.h"


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
  GEngine->AddOnScreenDebugMessage(-1,            // Key (-1 = nuevo mensaje cada vez)
                                   5.0f,          // Tiempo en segundos
                                   FColor::Green, // Color
                                   TEXT("Ground Movement State"));
}

void UGroundMovementState::Update(float _DeltaTime)
{

}

void UGroundMovementState::HandleInput(const FInputActionValue& _inputValue)
{
  APlayerCharacter* PlayerOwner = GetOwnerAs<APlayerCharacter>();
  if (PlayerOwner)
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
  APlayerCharacter* PlayerOwner = GetOwnerAs<APlayerCharacter>();
  if (PlayerOwner)
  {
    PlayerOwner->Dash();
  }
  GEngine->AddOnScreenDebugMessage(-1,            // Key (-1 = nuevo mensaje cada vez)
                                   5.0f,          // Tiempo en segundos
                                   FColor::Green, // Color
                                   TEXT("Dashing State"));
}

void UDashingState::Update(float _DeltaTime)
{

}

void UDashingState::Exit()
{
  APlayerCharacter* PlayerOwner = GetOwnerAs<APlayerCharacter>();
  if (PlayerOwner)
  {
    PlayerOwner->StopDash();
  }

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
