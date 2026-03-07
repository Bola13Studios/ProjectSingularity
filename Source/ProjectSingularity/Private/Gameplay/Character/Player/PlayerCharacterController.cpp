// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Character/Player/PlayerCharacterController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		inputSubsystem->AddMappingContext(m_GameplayIMC, 0);
	}
}
