/************************************************************************
 * @description: Image widget with support for resizing through the parent ResizableUserWidget.
 * @author: Jaime Paramo
 * @date: 10/03/2026
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "ProjectSingularity/Public/UI/ResizableUserWidget.h"
#include "CustomImageWidget.generated.h"

// Components
class UImage;

UCLASS()
class PROJECTSINGULARITY_API UCustomImageWidget : public UResizableUserWidget {
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
   * @brief Reference to the image component inside this widget. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pImage;
#pragma endregion
};