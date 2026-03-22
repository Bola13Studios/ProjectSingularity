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