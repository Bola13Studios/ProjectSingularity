#include "ProjectSingularity/Public/UI/GameHUDSetUp.h"

#include "ProjectSingularity/Public/UI/LeaderboardWidget.h"
#include "ProjectSingularity/Public/UI/PlayerNameWidget.h"
#include "ProjectSingularity/Public/Systems/BaseGameInstance.h"
#include "ProjectSingularity/Public/Systems/GameManagerSubsystem.h"
#include "ProjectSingularity/Public/Systems/PlayerSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HealthComponent.h"
#include "Components/Hype/HypeComponent.h"
#include "Components/Hype/PopularityComponent.h"
#include "Gameplay/Character/Player/PlayerCharacter.h"
#include "Systems/SpawnManager.h"

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

  if (m_bDebugShowLeaderboard)
    GetWorldTimerManager().SetTimer(m_debugLeaderboardTimer, this, &AGameHUDSetUp::ShowLeaderboard, 5.0f, false);

  if (USpawnManager* spawnManager = GetWorld()->GetSubsystem<USpawnManager>())
  {
    spawnManager->OnStateChange.AddDynamic(this, &AGameHUDSetUp::OnSpawnManagerStateChanged);
    UE_LOG(LogTemp, Warning, TEXT("[HUD] SpawnManager found and bound."));
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("[HUD] SpawnManager subsystem NOT found."));
  }

  if (UBaseGameInstance* gi = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    if (gi->m_bSkipNameEntry)
    {
      gi->m_bSkipNameEntry = false;

      if (APlayerController* pc = GetOwningPlayerController())
      {
        pc->SetInputMode(FInputModeGameOnly());
        pc->bShowMouseCursor = false;
      }

      InitializeAllGameMenus(false);
      return;
    }
  }

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

  FLeaderboardEntry* existing = saveGame->m_leaderboardEntries.FindByPredicate(
    [&playerName](const FLeaderboardEntry& e) { return e.playerName == playerName; });

  if (existing)
  {
    existing->score = score;
  }
  else
  {
    FLeaderboardEntry entry;
    entry.playerName = playerName;
    entry.score = score;
    saveGame->m_leaderboardEntries.Add(entry);
  }

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

  int32 score = 0;
  if (APawn* pawn = m_playerController->GetPawn())
  {
    if (UHypeComponent* hypeComp = pawn->FindComponentByClass<UHypeComponent>())
    {
      score = hypeComp->GetHype();
    }
  }
  SaveSessionToLeaderboard(score);

  TArray<FLeaderboardEntry> entries = LoadLeaderboard();

  m_leaderboardWidget = CreateWidget<ULeaderboardWidget>(m_playerController, m_leaderboardWidgetClass);
  if (!m_leaderboardWidget) return;

  m_leaderboardWidget->OnQuitGame.AddDynamic(this, &AGameHUDSetUp::OnLeaderboardQuitGame);
  m_leaderboardWidget->OnTryAgain.AddDynamic(this, &AGameHUDSetUp::OnLeaderboardTryAgain);
  m_leaderboardWidget->OnTryAgainNewPlayer.AddDynamic(this, &AGameHUDSetUp::OnLeaderboardTryAgainNewPlayer);

  m_leaderboardWidget->AddToViewport();
  m_leaderboardWidget->ShowWidget();
  m_leaderboardWidget->PopulateLeaderboard(entries);

  UGameplayStatics::SetGamePaused(GetWorld(), true);

  const FInputModeUIOnly inputMode = {};
  m_playerController->SetInputMode(inputMode);
  m_playerController->bShowMouseCursor = true;
}

void AGameHUDSetUp::OnLeaderboardQuitGame()
{
  UKismetSystemLibrary::QuitGame(GetWorld(), m_playerController, EQuitPreference::Quit, false);
}

void AGameHUDSetUp::OnLeaderboardTryAgain()
{
  if (UBaseGameInstance* gi = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    gi->m_bSkipNameEntry = true;
  }

  UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true)));
}

void AGameHUDSetUp::OnLeaderboardTryAgainNewPlayer()
{
  if (UBaseGameInstance* gi = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    gi->m_bSkipNameEntry = false;
  }

  UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true)));
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