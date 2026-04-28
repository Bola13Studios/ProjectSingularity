/************************************************************************
 * @description: Widget shown at the start of a session for the player
 *               to enter their display name.
 * @author: Serra
 * @date: 14/04/2026
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UI/CustomUserWidget.h"
#include "PlayerNameWidget.generated.h"

class UEditableTextBox;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerNameConfirmed, const FString&, sPlayerName);

static constexpr int32 MaxPlayerNameLength = 15;

UCLASS()
class PROJECTSINGULARITY_API UPlayerNameWidget : public UCustomUserWidget
{
  GENERATED_BODY()

#pragma region | VARIABLES

public:
  /**
   * @brief Broadcast when the player confirms their name. Carries the entered name.
   */
  UPROPERTY(BlueprintAssignable, Category = "Project Singularity|UI|Player Name")
  FOnPlayerNameConfirmed OnPlayerNameConfirmed;

private:
  /**
   * @brief Text input field where the player types their name. Must be bound in UMG.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UEditableTextBox> m_pNameInputBox;

  /**
   * @brief Confirm button that submits the entered name. Must be bound in UMG.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UButton> m_pConfirmButton;

#pragma endregion

#pragma region | METHODS

protected:
  /**
   * @brief Binds widget events on construction.
   */
  virtual void NativeConstruct() override;

private:
  /**
   * @brief Called on every keystroke to enforce the character limit in real time.
   */
  UFUNCTION()
  void OnNameTextChanged(const FText& text);

  /**
   * @brief Called when the player commits text (e.g. pressing Enter).
   */
  UFUNCTION()
  void OnNameTextCommitted(const FText& text, ETextCommit::Type commitMethod);

  /**
   * @brief Called when the player clicks the confirm button.
   *        Saves the name to the session and broadcasts the delegate.
   */
  UFUNCTION()
  void OnConfirmClicked();

#pragma endregion
};
