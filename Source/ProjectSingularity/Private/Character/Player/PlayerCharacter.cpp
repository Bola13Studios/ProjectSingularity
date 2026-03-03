// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"

APlayerCharacter::APlayerCharacter():
	ABaseCharacter()
{
	m_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	if (IsValid(m_camera))
	{
		m_camera->SetupAttachment(RootComponent);
		m_camera->bUsePawnControlRotation = true;
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
