// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUDSetUp.h"

void AGameHUDSetUp::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UGameInstance>(GetGameInstance());

	// Start focused on gameplay
	if (APlayerController* PC = GetOwningPlayerController())
	{
		const FInputModeGameOnly InputMode = {};
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}

	// Initialize all the game menus in the game
	InitializeAllGameMenus();
}

void AGameHUDSetUp::InitializeAllGameMenus()
{
	// HUD
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}
