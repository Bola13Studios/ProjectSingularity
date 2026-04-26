#include "ProjectSingularity/Public/UI/GameHUDSetUp.h"

#include "ProjectSingularity/Public/UI/LeaderboardWidget.h"
#include "ProjectSingularity/Public/UI/PlayerNameWidget.h"
#include "ProjectSingularity/Public/Systems/BaseGameInstance.h"
#include "ProjectSingularity/Public/Systems/GameManagerSubsystem.h"
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

  if (UGameManagerSubsystem* gameManager = GetGameInstance()->GetSubsystem<UGameManagerSubsystem>())
  {
    gameManager->OnGameStateChanged.AddDynamic(this, &AGameHUDSetUp::OnGameStateChanged);
  }

  if (ASpawnerManager* spawnerManager = Cast<ASpawnerManager>(
    UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnerManager::StaticClass())))
  {
    spawnerManager->OnStateChange.AddDynamic(this, &AGameHUDSetUp::OnSpawnManagerStateChanged);
    UE_LOG(LogTemp, Warning, TEXT("[HUD] SpawnerManager found and bound."));
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("[HUD] SpawnerManager NOT found in level."));
  }

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

void AGameHUDSetUp::OnSpawnManagerStateChanged(ESpawnManagerState state)
{
  UE_LOG(LogTemp, Warning, TEXT("[HUD] SpawnManager state: %d"), (uint8)state);

  if (state != ESpawnManagerState::FINISHED) return;

  UE_LOG(LogTemp, Warning, TEXT("[HUD] All rounds finished — triggering GAMEOVER."));

  if (UGameManagerSubsystem* gameManager = GetGameInstance()->GetSubsystem<UGameManagerSubsystem>())
  {
    gameManager->SetGameState(EGameState::GAMEOVER);
  }
}

void AGameHUDSetUp::OnGameStateChanged(EGameState newState)
{
  if (newState == EGameState::GAMEOVER)
  {
    ShowLeaderboard();
  }
}

void AGameHUDSetUp::SaveSessionToLeaderboard(int32 score)
{
  FString playerName = TEXT("Unknown");
  if (UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    playerName = gameInstance->m_sessionData.m_sPlayerName;
  }

  UPlayerSaveGame* saveGame = nullptr;
  if (UGameplayStatics::DoesSaveGameExist(TEXT("LeaderboardData"), 0))
  {
    saveGame = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("LeaderboardData"), 0));
  }
  if (!saveGame)
  {
    saveGame = Cast<UPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass()));
  }
  if (!saveGame) return;

  FLeaderboardEntry entry;
  entry.playerName = playerName;
  entry.score = score;
  saveGame->m_leaderboardEntries.Add(entry);

  saveGame->m_leaderboardEntries.Sort([](const FLeaderboardEntry& a, const FLeaderboardEntry& b)
  {
    return a.score > b.score;
  });

  UGameplayStatics::SaveGameToSlot(saveGame, TEXT("LeaderboardData"), 0);
}

TArray<FLeaderboardEntry> AGameHUDSetUp::LoadLeaderboard() const
{
  if (!UGameplayStatics::DoesSaveGameExist(TEXT("LeaderboardData"), 0))
    return {};

  UPlayerSaveGame* saveGame = Cast<UPlayerSaveGame>(
    UGameplayStatics::LoadGameFromSlot(TEXT("LeaderboardData"), 0)
  );

  return saveGame ? saveGame->m_leaderboardEntries : TArray<FLeaderboardEntry>{};
}

void AGameHUDSetUp::ShowLeaderboard()
{
  UE_LOG(LogTemp, Warning, TEXT("[HUD] ShowLeaderboard called. WidgetClass=%s, Controller=%s"),
    m_leaderboardWidgetClass ? *m_leaderboardWidgetClass->GetName() : TEXT("NULL"),
    m_playerController ? TEXT("OK") : TEXT("NULL"));

  if (!m_leaderboardWidgetClass || !m_playerController) return;

  constexpr int32 PlaceholderScore = 1000;
  SaveSessionToLeaderboard(PlaceholderScore);

  TArray<FLeaderboardEntry> entries = LoadLeaderboard();

  m_leaderboardWidget = CreateWidget<ULeaderboardWidget>(m_playerController, m_leaderboardWidgetClass);
  if (!m_leaderboardWidget) return;

  m_leaderboardWidget->PopulateLeaderboard(entries);
  m_leaderboardWidget->AddToViewport();
  m_leaderboardWidget->ShowWidget();

  const FInputModeUIOnly inputMode = {};
  m_playerController->SetInputMode(inputMode);
  m_playerController->bShowMouseCursor = true;
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