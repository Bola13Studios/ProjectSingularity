#include "Systems/LogManagerSubsystem.h"
#include "Systems/GameManagerSubsystem.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

void ULogManagerSubsystem::Initialize(FSubsystemCollectionBase& _rCollection)
{
  Super::Initialize(_rCollection);

  UE_LOG(LogTemp, Log, TEXT("Logger Initialized"));
}

void ULogManagerSubsystem::Deinitialize()
{
  if (UGameInstance* gameInstance = GetGameInstance())
  {
    if (UGameManagerSubsystem* gameManager = gameInstance->GetSubsystem<UGameManagerSubsystem>())
    {
      SaveSession(gameManager->GetAllData());
    }
  }
  
  SaveToFile(); // we automatically save to a file upon closure

  Super::Deinitialize();
}

void ULogManagerSubsystem::LogEvent(const FString& _event)
{
  FString timestamp = FDateTime::Now().ToString();
  m_loggedEvents.Add(FString::Printf(TEXT("[%s] %s"), *timestamp, *_event));
}

void ULogManagerSubsystem::SaveToFile()
{
  if (m_loggedEvents.Num() == 0) return; // no events to log here

  FString output   = FString::Join(m_loggedEvents, TEXT("\n"));
  FString filePath = GenerateFilePath("Events");

  FFileHelper::SaveStringToFile(output, *filePath);

  UE_LOG(LogTemp, Log, TEXT("Saved log to: %s"), *filePath);
}

void ULogManagerSubsystem::SaveSession(const FSessionData& _data)
{
  FString output;

  for (const auto& pair : _data.stats)
  {
    output += FString::Printf(TEXT("%s : %f\n"), *pair.Key.ToString(), pair.Value);
  }

  FString filePath = GenerateFilePath("Stats");
  FFileHelper::SaveStringToFile(output, *filePath);
}

FString ULogManagerSubsystem::GenerateFilePath(FString _folder) const
{
  FString _dir = FPaths::ProjectSavedDir() + TEXT("SessionLogs/") + _folder + TEXT("/");

  IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();

  if (!platformFile.DirectoryExists(*_dir))
    platformFile.CreateDirectoryTree(*_dir); // we create the dir if it does not exists

  FString _fileName = FString::Printf(TEXT("Session_%s.txt"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));

  return _dir + _fileName;
}
