#include "ProjectSingularity/Public/UI/GameHUDSetUp.h"

#include "Components/HealthComponent.h"
#include "Components/Hype/HypeComponent.h"
#include "Components/Hype/PopularityComponent.h"

#pragma region | PROTECTED METHODS

void AGameHUDSetUp::BeginPlay()
{
  Super::BeginPlay();

  m_gameInstance = Cast<UGameInstance>(GetGameInstance());
  m_playerController = GetOwningPlayerController();

  // Start focused on gameplay
  if (APlayerController* playerController = GetOwningPlayerController())
  {
    const FInputModeGameOnly inputMode = {};
    playerController->SetInputMode(inputMode);
    playerController->bShowMouseCursor = false;
  }

  // Initialize all game menus
  InitializeAllGameMenus();
}

#pragma endregion

#pragma region | PRIVATE METHODS

void AGameHUDSetUp::InitializeAllGameMenus()
{
  if (!m_playerController) m_playerController = GetOwningPlayerController();

  if (!m_playerController) return;

  // Create main HUD widget
  if (m_hudWidgetClass)
  {
    m_hudWidget = CreateWidget<UHUDWidget>(m_playerController, m_hudWidgetClass);

    if (m_hudWidget)
    {
      m_hudWidget->AddToViewport();
      TryBindHUDToPawn();
    }
  }
}

void AGameHUDSetUp::TryBindHUDToPawn() const
{
  if (!m_playerController || !m_hudWidget) return;

  APawn* pawn = m_playerController->GetPawn();
  if (!pawn) return;

  if (UHealthComponent* healthComp = pawn->FindComponentByClass<UHealthComponent>())
  {
    m_hudWidget->BindToHealthComponent(healthComp);
  }

  if (UHypeComponent* hypeComp = pawn->FindComponentByClass<UHypeComponent>())
  {
    m_hudWidget->BindToHypeComponent(hypeComp);
  }

  if (UPopularityComponent* popularityComp = pawn->FindComponentByClass<UPopularityComponent>())
  {
    m_hudWidget->BindToPopularityComponent(popularityComp);
  }
}

#pragma endregion