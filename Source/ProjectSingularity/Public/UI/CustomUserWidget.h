/************************************************************************
 * @description: Base class extending Unreal Engine's UserWidget.
 * @author: Jaime Paramo
 * @date: 10/03/2026
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomUserWidget.generated.h"

UCLASS()
class PROJECTSINGULARITY_API UCustomUserWidget : public UUserWidget {
  GENERATED_BODY()

public:
#pragma region Visibility
  /**
   * @brief Makes this widget visible and updates all contained custom widgets accordingly.
   */
  UFUNCTION(BlueprintCallable, Category = "Project Singularity|UI|Visibility", meta = (DisplayName = "Show Widget"))
  virtual void ShowWidget();
  /**
   * @brief Hides this widget and updates all contained custom widgets to reflect the change.
   */
  UFUNCTION(BlueprintCallable, Category = "Project Singularity|UI|Visibility", meta = (DisplayName = "Hide Widget"))
  virtual void HideWidget();

  /**
   * @brief Returns whether the widget is currently visible on screen.
   * @return True if the widget is visible, false otherwise.
   */
  UFUNCTION(BlueprintCallable, Category = "Project Singularity|UI|Visibility", meta = (DisplayName = "Is Widget Visible"))
  virtual bool IsWidgetVisible();
#pragma endregion

protected:
#pragma region Native Functions
  /**
   * @brief Called when the widget is constructed.
   */
  virtual void NativeConstruct() override;
#pragma endregion

#pragma region Containers
  /**
   * @brief Stores references to other custom user widgets contained within this widget.
   */
  UPROPERTY()
  TArray<TObjectPtr<UCustomUserWidget>> m_lContainedWidgets;
#pragma endregion

#pragma region Flags
  /**
   * @brief Indicates whether the widget is currently visible. Used internally to track visibility state.
   */
  bool m_bWidgetVisible = false;
#pragma endregion
};