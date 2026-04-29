/************************************************************************
 * @description: This class generates a file holding logs and various statistics
 * @author: Josephine Esposito
 * @date: 24/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Systems/SessionData.h"
#include "LogManagerSubsystem.generated.h"

/**
 * This class generates a file holding logs and various statistics
 */
UCLASS()
class PROJECTSINGULARITY_API ULogManagerSubsystem : public UGameInstanceSubsystem
{
  GENERATED_BODY()

private:
  /**
   * @brief Saves the list of events to load into the file
   */
  TArray<FString> m_loggedEvents;

public:
  /**
   * @brief Called to initialize the subsystem
   * @param _rCollection
   */
  virtual void Initialize(FSubsystemCollectionBase& _rCollection) override;

  /**
   * @brief Called to deinitialize the subsystem
   */
  virtual void Deinitialize() override;

  /**
   * @brief Will save a line into the log files
   * @param _worldContext the World context
   * @param _msg The message to save
   */
  static void Log(UObject* _worldContext, const TCHAR* _msg);

  /**
   * @brief Will log an event into the file
   * @param _event The event to save
   */
  void LogEvent(const FString& _event);

  /**
   * @brief Will save all the saved events into a file
   */
  void SaveToFile();

  /**
   * @brief Will save all the stats into a json file
   * @param _data The data to save
   */
  void SaveSessionJSON(const FSessionData& _data);

  UFUNCTION()
  void HandleGameStateChanged(EGameState eNewGameState);

private:
  /**
   * @brief Will generate the path for the file to generate
   * @param _folder The name of the folder
   * @return The string path
   */
  FString GenerateFilePath(FString _folder) const;

  /**
   * @brief Will get the saved version
   * @return The string containig the saved version
   */
  FString GetVersion() const;
};
