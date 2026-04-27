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
class UButton;
class ULeaderboardRowWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeaderboardAction);

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

  UPROPERTY(BlueprintAssignable)
  FOnLeaderboardAction OnQuitGame;

  UPROPERTY(BlueprintAssignable)
  FOnLeaderboardAction OnTryAgain;

  UPROPERTY(BlueprintAssignable)
  FOnLeaderboardAction OnTryAgainNewPlayer;

private:
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UScrollBox> m_pEntriesBox;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UButton> m_pQuitButton;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UButton> m_pTryAgainButton;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UButton> m_pTryAgainNewPlayerButton;

#pragma endregion

#pragma region | METHODS

public:
  /**
   * @brief Clears and rebuilds the entry list from the provided data.
   * @param entries Sorted leaderboard entries to display.
   */
  void PopulateLeaderboard(const TArray<FLeaderboardEntry>& entries);

protected:
  virtual void NativeConstruct() override;

private:
  UFUNCTION()
  void OnQuitClicked();

  UFUNCTION()
  void OnTryAgainClicked();

  UFUNCTION()
  void OnTryAgainNewPlayerClicked();

#pragma endregion
};
