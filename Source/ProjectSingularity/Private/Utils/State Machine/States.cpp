// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/State Machine/States.h"
#include "Gameplay/Weapons/WeaponBase.h"
#include "Gameplay/Character/BaseCharacter.h"


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

}

void UGroundMovementState::Update(float _DeltaTime)
{
}

void UGroundMovementState::HandleInput(const FInputActionValue& _inputValue)
{
}

void UGroundMovementState::Exit()
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