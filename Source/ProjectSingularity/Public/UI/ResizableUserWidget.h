/************************************************************************
 * @description: UserWidget with built-in support for resizing through a configurable SizeBox.
 * @author: Jaime Paramo
 * @date: 10/03/2026
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "ProjectSingularity/Public/UI/CustomUserWidget.h"
#include "ResizableUserWidget.generated.h"

 // Components
class USizeBox;

UCLASS()
class PROJECTSINGULARITY_API UResizableUserWidget : public UCustomUserWidget {
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

#pragma region Layout
  /**
   * @brief Returns the desired size of the widget in pixels (Width = X, Height = Y).
   * @return Current widget size as a FVector2D.
   */
  UFUNCTION(BlueprintCallable, Category = "Project Singularity/UI/Layout", meta = (DisplayName = "Get Widget Size"))
  FVector2D GetWidgetSize() const;
  /**
   * @brief Sets the desired size of the widget in pixels (Width = X, Height = Y). Updates the SizeBox overrides if available.
   * @param _vNewSize New size to assign to the widget.
   */
  UFUNCTION(BlueprintCallable, Category = "Project Singularity/UI/Layout", meta = (DisplayName = "Set Widget Size"))
  void SetWidgetSize(FVector2D _vNewSize);

  /**
   * @brief Returns the desired width of the widget in pixels.
   * @return Current widget width as a float.
   */
  UFUNCTION(BlueprintCallable, Category = "Project Singularity/UI/Layout", meta = (DisplayName = "Get Widget Width"))
  float GetWidgetWidth() const;
  /**
   * @brief Sets the desired width of the widget in pixels. Updates the SizeBox overrides if available.
   * @param _vNewSize New width to assign to the widget.
   */
  UFUNCTION(BlueprintCallable, Category = "Project Singularity/UI/Layout", meta = (DisplayName = "Set Widget Width"))
  void SetWidgetWidth(float _fNewWidth);

  /**
    * @brief Returns the desired height of the widget in pixels.
    * @return Current widget height as a float.
    */
  UFUNCTION(BlueprintCallable, Category = "Project Singularity/UI/Layout", meta = (DisplayName = "Get Widget Height"))
  float GetWidgetHeight() const;
  /**
   * @brief Sets the desired height of the widget in pixels. Updates the SizeBox overrides if available.
   * @param _vNewSize New height to assign to the widget.
   */
  UFUNCTION(BlueprintCallable, Category = "Project Singularity/UI/Layout", meta = (DisplayName = "Set Widget Height"))
  void SetWidgetHeight(float _fNewHeight);
#pragma endregion

protected:
#pragma region Native Functions
  /**
   * @brief Called when the widget is constructed.
   */
  virtual void NativeConstruct() override;
  /**
   * @brief Synchronizes editable properties between editor and runtime.
   */
  virtual void SynchronizeProperties() override;
#pragma endregion

#pragma region Components
  /**
   * @brief Required SizeBox that defines the widget's dimensions. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<USizeBox> m_pRootSizeBox;
#pragma endregion

#pragma region Properties
  /**
   * @brief Default size of the widget (Width = X, Height = Y), editable in Blueprints.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Singularity|UI|Layout", meta = (DisplayName = "Widget Size", ClampMin = "0.0"))
  FVector2D m_vWidgetSize = FVector2D(100.f, 100.f);
#pragma endregion

private:
#pragma region Helper Functions
  /**
   * @brief Ensures the SizeBox correctly drives the widget size.
   *        For CanvasPanelSlot, enables auto-sizing so the SizeBox dimensions are respected.
   *        For other panel types, the SizeBox size is applied automatically.
   */
  void UpdateSizeToContent();
#pragma endregion 
};