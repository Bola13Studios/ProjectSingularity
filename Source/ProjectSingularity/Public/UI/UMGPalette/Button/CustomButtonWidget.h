/************************************************************************
 * @description: Button widget with built-in text handling and support for resizing through the parent ResizableUserWidget.
 * @author: Jaime Paramo
 * @date: 10/03/2026
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "ProjectSingularity/Public/UI/ResizableUserWidget.h"
#include "CustomButtonWidget.generated.h"

// Components
class UButton;
class UTextBlock;

UCLASS()
class PROJECTSINGULARITY_API UCustomButtonWidget : public UResizableUserWidget {
  GENERATED_BODY()

public:
#pragma region Visibility
  /**
   * @brief Makes this widget visible and updates all contained custom widgets accordingly.
   */
  virtual void ShowWidget() override;
  /**
   * @brief Hides this widget and updates all contained custom widgets to reflect the change.
   */
  virtual void HideWidget() override;
#pragma endregion

protected:
#pragma region Native Functions
  /**
   * @brief Called when the widget is constructed.
   */
  virtual void NativeConstruct() override;
  /**
   * @brief Stores references to other custom user widgets contained within this widget.
   */
  virtual void SynchronizeProperties() override;
#pragma endregion

#pragma region Components
  /**
   * @brief Reference to the button component inside this widget. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UButton> m_pButton;

  /**
   * @brief Reference to the TextBlock displaying the button's label. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pButtonText;
#pragma endregion

#pragma region Properties
  /**
   * @brief The text that will appear on the button.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Singularity|UI|Properties", meta = (DisplayName = "Button Text"));
  FText m_sButtonText;
#pragma endregion

#pragma region Flags
  /**
   * @brief Whether the button text should be shown. Controls the visibility of the TextBlock.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Singularity|UI|Properties", meta = (DisplayName = "Show Text"))
  bool m_bShowText = false;
#pragma endregion

private:
#pragma region Helper Functions
  /**
   * @brief Applies visibility and text updates to the TextBlock based on the current editor/runtime settings.
   */
  void ApplyTextSettings();
#pragma endregion 
};