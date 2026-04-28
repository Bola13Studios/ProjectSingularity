/************************************************************************
 * @description: Widget that displays a single leaderboard entry row,
 *               showing rank, player name and score.
 * @author: Serra
 * @date: 26/04/2026
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UI/CustomUserWidget.h"
#include "LeaderboardRowWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTSINGULARITY_API ULeaderboardRowWidget : public UCustomUserWidget
{
  GENERATED_BODY()

#pragma region | VARIABLES

private:
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pRankText;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pNameText;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pScoreText;

#pragma endregion

#pragma region | METHODS

public:
  /**
   * @brief Fills in the row with rank, name, and score data.
   * @param rank    Position in the leaderboard (1-based).
   * @param playerName Display name of the player.
   * @param score   Score obtained during the session.
   */
  void InitRow(int32 rank, const FString& playerName, int32 score);

#pragma endregion
};
