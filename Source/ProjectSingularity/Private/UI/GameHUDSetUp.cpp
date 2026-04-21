// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectSingularity/Public/UI/GameHUDSetUp.h"

#include "Components/HealthComponent.h"
#include "Components/Hype/HypeComponent.h"
#include "Components/Hype/PopularityComponent.h"

void AGameHUDSetUp::BeginPlay()
{
  Super::BeginPlay();

  GameInstance = Cast<UGameInstance>(GetGameInstance());
  PlayerController = GetOwningPlayerController();

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
  if (!PlayerController) PlayerController = GetOwningPlayerController();

  if (!PlayerController) return;

  // HUD

  if (HUDWidgetClass)
  {
    HUDWidget = CreateWidget<UHUDWidget>(PlayerController, HUDWidgetClass);
    if (HUDWidget)
    {
      HUDWidget->AddToViewport();
      TryBindHUDToPawn();
    }
  }
}

void AGameHUDSetUp::TryBindHUDToPawn() const
{
  if (!PlayerController || !HUDWidget) return;

  APawn* Pawn = PlayerController->GetPawn();
  if (!Pawn) return;

  if (UHealthComponent* HealthComp = Pawn->FindComponentByClass<UHealthComponent>())
  {
    HUDWidget->BindToHealthComponent(HealthComp);
  }

  if (UHypeComponent* HypeComp = Pawn->FindComponentByClass<UHypeComponent>())
  {
    HUDWidget->BindToHypeComponent(HypeComp);
  }

  if (UPopularityComponent* popularityComponent = Pawn->FindComponentByClass<UPopularityComponent>())
  {
    HUDWidget->BindToPopularityComponent(popularityComponent);
  }
}
