/************************************************************************
 * @description: HUD widget that displays a welcome message with the
 *               player's name and a random player number.
 * @author: Serra
 * @date: 14/04/2026
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UI/CustomUserWidget.h"
#include "WelcomeWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTSINGULARITY_API UWelcomeWidget : public UCustomUserWidget
{
  GENERATED_BODY()

#pragma region | VARIABLES

private:
  /**
   * @brief Text block that displays the full welcome message. Must be bound in UMG.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pWelcomeText;

  /**
   * @brief Random player number generated once on construction (0-999).
   */
  int32 m_iPlayerNumber = 0;

#pragma endregion

#pragma region | METHODS

protected:
  /**
   * @brief Generates the random number and builds the welcome text on construction.
   */
  virtual void NativeConstruct() override;

private:
  /**
   * @brief Builds and applies the welcome string to the text block.
   */
  void RefreshWelcomeText();

#pragma endregion
};
