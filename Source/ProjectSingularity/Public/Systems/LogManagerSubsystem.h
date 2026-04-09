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
  TArray<FString> m_loggedEvents;

public:
  virtual void Initialize(FSubsystemCollectionBase& _rCollection) override;

  virtual void Deinitialize() override;

  void LogEvent(const FString& _event);

  void SaveToFile();

  void SaveSession(const FSessionData& _data);

  void SaveSessionJSON(const FSessionData& _data);

private:
  FString GenerateFilePath(FString _folder) const;

  FString GetVersion() const;
};
