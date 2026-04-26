/************************************************************************
 * @description: Widget that displays the full local leaderboard at the
 *               end of a session, listing all players ranked by score.
 * @author: Serra
 * @date: 26/04/2026
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UI/CustomUserWidget.h"
#include "Systems/PlayerSaveGame.h"
#include "LeaderboardWidget.generated.h"

class UScrollBox;
class ULeaderboardRowWidget;

UCLASS()
class PROJECTSINGULARITY_API ULeaderboardWidget : public UCustomUserWidget
{
  GENERATED_BODY()

#pragma region | VARIABLES

public:
  /**
   * @brief Row widget class to instantiate for each leaderboard entry. Set in Blueprint.
   */
  UPROPERTY(EditDefaultsOnly, Category = "UI")
  TSubclassOf<ULeaderboardRowWidget> m_rowWidgetClass;

private:
  /**
   * @brief Scroll container that holds the generated row widgets. Must be bound in UMG.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UScrollBox> m_pEntriesBox;

#pragma endregion

#pragma region | METHODS

public:
  /**
   * @brief Clears and rebuilds the entry list from the provided data.
   * @param entries Sorted leaderboard entries to display.
   */
  void PopulateLeaderboard(const TArray<FLeaderboardEntry>& entries);

#pragma endregion
};
