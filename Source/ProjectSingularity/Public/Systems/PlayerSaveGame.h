/************************************************************************
 * @description: Save game class that persists the player's session data
 *               between play sessions.
 * @author: Serra
 * @date: 14/04/2026
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FLeaderboardEntry
{
  GENERATED_BODY()

  UPROPERTY()
  FString playerName;

  UPROPERTY()
  int32 score = 0;
};

UCLASS()
class PROJECTSINGULARITY_API UPlayerSaveGame : public USaveGame
{
  GENERATED_BODY()

public:
  /**
   * @brief The player's display name saved from the previous session.
   */
  UPROPERTY()
  FString m_sPlayerName;

  /**
   * @brief All leaderboard entries recorded on this machine, sorted by score descending.
   */
  UPROPERTY()
  TArray<FLeaderboardEntry> m_leaderboardEntries;
};
