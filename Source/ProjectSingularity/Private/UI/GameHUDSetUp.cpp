#include "ProjectSingularity/Public/UI/GameHUDSetUp.h"

#include "ProjectSingularity/Public/UI/PlayerNameWidget.h"
#include "ProjectSingularity/Public/Systems/BaseGameInstance.h"
#include "ProjectSingularity/Public/Systems/PlayerSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HealthComponent.h"
#include "Components/Hype/HypeComponent.h"
#include "Components/Hype/PopularityComponent.h"
#include "Gameplay/Character/Player/PlayerCharacter.h"

#pragma region | PROTECTED METHODS

void AGameHUDSetUp::BeginPlay()
{
  Super::BeginPlay();

  m_gameInstance = Cast<UGameInstance>(GetGameInstance());
  m_playerController = GetOwningPlayerController();

  // Try to load a saved player name from a previous session
  if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerData"), 0))
  {
    UPlayerSaveGame* saveGame = Cast<UPlayerSaveGame>(
      UGameplayStatics::LoadGameFromSlot(TEXT("PlayerData"), 0)
    );

    if (saveGame && !saveGame->m_sPlayerName.IsEmpty())
    {
      if (UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
      {
        gameInstance->m_sessionData.m_sPlayerName = saveGame->m_sPlayerName;
      }

      // Name already known — skip the entry screen and go straight to gameplay
      if (APlayerController* playerController = GetOwningPlayerController())
      {
        const FInputModeGameOnly inputMode = {};
        playerController->SetInputMode(inputMode);
        playerController->bShowMouseCursor = false;
      }

      InitializeAllGameMenus(false);
      return;
    }
  }

  // No saved name — pause and show the name entry screen
  UGameplayStatics::SetGamePaused(GetWorld(), true);

  if (APlayerController* playerController = GetOwningPlayerController())
  {
    const FInputModeUIOnly inputMode = {};
    playerController->SetInputMode(inputMode);
    playerController->bShowMouseCursor = true;
  }

  InitializeAllGameMenus(true);
}

#pragma endregion

#pragma region | PRIVATE METHODS

void AGameHUDSetUp::InitializeAllGameMenus(bool _bShowNameEntry)
{
  if (!m_playerController)
  {
    m_playerController = GetOwningPlayerController();
  }

  if (!m_playerController)
  {
    return;
  }

  if (_bShowNameEntry && m_playerNameWidgetClass)
  {
    m_playerNameWidget = CreateWidget<UPlayerNameWidget>(m_playerController, m_playerNameWidgetClass);

    if (m_playerNameWidget)
    {
      m_playerNameWidget->OnPlayerNameConfirmed.AddDynamic(this, &AGameHUDSetUp::OnPlayerNameConfirmed);
      m_playerNameWidget->AddToViewport();
      m_playerNameWidget->ShowWidget();
    }
  }

  if (m_hudWidgetClass)
  {
    m_hudWidget = CreateWidget<UHUDWidget>(m_playerController, m_hudWidgetClass);

    if (m_hudWidget)
    {
      TryBindHUDToPawn();

      // If name entry is skipped, show the HUD immediately
      if (!_bShowNameEntry)
      {
        m_hudWidget->AddToViewport();
      }
    }
  }
}

void AGameHUDSetUp::OnPlayerNameConfirmed(const FString& sPlayerName)
{
  // Unpause and switch to gameplay input now that the name has been entered
  UGameplayStatics::SetGamePaused(GetWorld(), false);

  if (m_playerController)
  {
    const FInputModeGameOnly inputMode = {};
    m_playerController->SetInputMode(inputMode);
    m_playerController->bShowMouseCursor = false;
  }

  // Show the HUD
  if (m_hudWidget)
  {
    m_hudWidget->AddToViewport();
  }
}

void AGameHUDSetUp::TryBindHUDToPawn() const
{
  if (!m_playerController || !m_hudWidget)
  {
    return;
  }

  APawn* pawn = m_playerController->GetPawn();
  if (!pawn)
  {
    return;
  }

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

  if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(pawn))
  {
    if (AWeaponBase* weapon = playerCharacter->GetWeapon())
    {
      m_hudWidget->BindToWeapon(weapon);
    }
  }
}

#pragma endregion