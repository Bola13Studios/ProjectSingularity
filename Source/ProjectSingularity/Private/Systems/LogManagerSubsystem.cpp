#include "Systems/LogManagerSubsystem.h"
#include "Systems/GameManagerSubsystem.h"
#include "Systems/BaseGameInstance.h"
#include <Misc/FileHelper.h>
#include <Misc/Paths.h>
#include <HAL/PlatformFileManager.h>
#include <Dom/JsonObject.h>
#include <Serialization/JsonWriter.h>
#include <Serialization/JsonSerializer.h>
#include "JsonObjectConverter.h"
#include "Utils/Types/GameStateEnum.h"

void ULogManagerSubsystem::Initialize(FSubsystemCollectionBase& _rCollection)
{
  Super::Initialize(_rCollection);

  UE_LOG(LogTemp, Log, TEXT("[MICHAEL.JSON] Logger Initialized!!"));

  if (auto* GameManager = GetGameInstance()->GetSubsystem<UGameManagerSubsystem>())
  {
    GameManager->OnGameStateChanged.AddDynamic(this, &ULogManagerSubsystem::HandleGameStateChanged);
  }
}

void ULogManagerSubsystem::Deinitialize()
{
  UGameInstance* GI = GetGameInstance();
  if (!GI)
  {
    Super::Deinitialize();
    return;
  }

  UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GI);
  if (!gameInstance)
  {
    Super::Deinitialize();
    return;
  }

  if (gameInstance->canSaveStats)
  {
    UE_LOG(LogTemp, Log, TEXT("[MICHAEL.JSON] Saving session stats..."));

    if (UGameManagerSubsystem* gameManager = GI->GetSubsystem<UGameManagerSubsystem>())
    {
      //SaveSessionJSON(gameManager->GetAllData());
      gameManager->OnGameStateChanged.RemoveAll(this);
    }
  }

  if (gameInstance->canSaveEvents)
  {
    UE_LOG(LogTemp, Log, TEXT("[MICHAEL.JSON] Saving session events..."));
    SaveToFile();
  }

  Super::Deinitialize();
}

void ULogManagerSubsystem::Log(UObject* _worldContext, const TCHAR* _msg)
{
  if (!_worldContext) return;

  UWorld* world = _worldContext->GetWorld();
  if (!world) return;

  UGameInstance* gameInstance = world->GetGameInstance();
  if (!gameInstance) return;

  if (ULogManagerSubsystem* logManager = gameInstance->GetSubsystem<ULogManagerSubsystem>())
  {
    logManager->LogEvent(_msg);
  }
}

void ULogManagerSubsystem::LogEvent(const FString& _event)
{
  FString timestamp = FDateTime::Now().ToString();
  m_loggedEvents.Add(FString::Printf(TEXT("[%s] %s"), *timestamp, *_event));
}

void ULogManagerSubsystem::SaveToFile()
{
  if (m_loggedEvents.Num() == 0) return; // no events to log here

  FString output = FString::Join(m_loggedEvents, TEXT("\n"));
  FString filePath = GenerateFilePath("Events") + TEXT(".txt");

  FFileHelper::SaveStringToFile(output, *filePath);

  UE_LOG(LogTemp, Log, TEXT("[MICHAEL.JSON] Saved log to: %s"), *filePath);
}

void ULogManagerSubsystem::SaveSessionJSON(const FSessionData& _data)
{
  FString output;

  if (FJsonObjectConverter::UStructToJsonObjectString(FSessionData::StaticStruct(), &_data, output, 0, 0, 0, nullptr,
                                                      true))
  {
    FString filePath = GenerateFilePath("Stats") + TEXT(".json");

    if (FFileHelper::SaveStringToFile(output, *filePath))
    {
      UE_LOG(LogTemp, Log, TEXT("[JSON] Saved to: %s"), *filePath);
    }
    else
    {
      UE_LOG(LogTemp, Error, TEXT("[JSON] Failed to write file"));
    }
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("[JSON] Serialization FAILED"));
  }
}

void ULogManagerSubsystem::HandleGameStateChanged(EGameState eNewGameState)
{
  UE_LOG(LogTemp, Warning, TEXT("GameState changed: %d"), (int32)eNewGameState);

  if (eNewGameState == EGameState::GAMEOVER)
  {
    UE_LOG(LogTemp, Warning, TEXT("GAME OVER -> Saving session data"));
    if (UGameInstance* GI = GetGameInstance())
    {
      if (UGameManagerSubsystem* gameManager = GI->GetSubsystem<UGameManagerSubsystem>())
      {
        SaveSessionJSON(gameManager->GetAllData());
      }
    }
  }
}

FString ULogManagerSubsystem::GenerateFilePath(FString _folder) const
{
  FString _dir = FPaths::ProjectSavedDir() + TEXT("SessionLogs/") + _folder + TEXT("/");

  IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();

  if (!platformFile.DirectoryExists(*_dir))
    platformFile.CreateDirectoryTree(*_dir); // we create the dir if it does not exists

  FString _fileName = FString::Printf(TEXT("Session_%s"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));

  return _dir + _fileName;
}

FString ULogManagerSubsystem::GetVersion() const
{
  if (UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    return gameInstance->m_version.ToString();
  }
  return FString();
}
