/************************************************************************
 * @description: Represents the widget for displaying the player's
 * popularity level and related information in the UI
 * @author: Josephine Esposito
 * @date: 08/04/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include <Blueprint/UserWidget.h>
#include "PopularityWidget.generated.h"

#pragma region | Forward Declarations
class UPopularityComponent;
class UTextBlock;
class UProgressBar;
#pragma endregion

/**
 * Represents the widget for displaying the player's
 * popularity level and related information in the UI
 */
UCLASS()
class PROJECTSINGULARITY_API UPopularityWidget : public UUserWidget
{
  GENERATED_BODY()

#pragma region | Variables

protected:
  /**
   * @brief Reference to the progress bar widget that displays the current popularity level as a percentage
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UProgressBar> m_popularityBar;

  /**
   * @brief Reference to the text block widget that displays the multiplier value
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_multiplierText;

private:
  /**
   * @brief Reference to the popularity component that this widget is bound to, used to retrieve popularity data and
   * update the UI accordingly
   */
  UPROPERTY()
  TObjectPtr<UPopularityComponent> m_popularityComponent;

#pragma endregion

public:
  /**
   * @brief Used to bind the widget to a specific popularity component, allowing it to listen for changes and update the
   * UI accordingly
   * @param _inPopularityComp The popularity component to bind to, which provides the data for the widget to display
   */
  UFUNCTION(BlueprintCallable)
  void BindToPopularityComponent(UPopularityComponent* _inPopularityComp);

protected:
  /**
   * @brief Called when the widget is constructed, used to perform any necessary setup such as binding to the popularity
   * component
   */
  virtual void NativeConstruct() override;

  /**
   * @brief Called when the widget is being destroyed, used to perform any necessary cleanup such as unbinding from the
   * popularity component
   */
  virtual void NativeDestruct() override;

private:
  /**
   * @brief Called whenever the popularity component broadcasts a change in popularity, used to refresh the UI elements
   * such as the progress bar and multiplier text to reflect the current popularity state
   */
  UFUNCTION()
  void RefreshUI();
};
