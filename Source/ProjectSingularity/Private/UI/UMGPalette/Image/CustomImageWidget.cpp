/************************************************************************
 * @description: Image widget with support for resizing through the parent ResizableUserWidget.
 * @author: Jaime Paramo
 * @date: 10/03/2026
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "ProjectSingularity/Public/UI/UMGPalette/Image/CustomImageWidget.h"

// Components
#include "Components/Image.h"

void UCustomImageWidget::ShowWidget() {
  Super::ShowWidget();

  // Ensure the image is visible when the widget is shown.
  if (IsValid(m_pImage)) m_pImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UCustomImageWidget::HideWidget() {
  Super::HideWidget();

  // Hide image when the widget is hidden.
  if (IsValid(m_pImage)) m_pImage->SetVisibility(ESlateVisibility::Hidden);
}

void UCustomImageWidget::NativeConstruct() { Super::NativeConstruct(); }

void UCustomImageWidget::SynchronizeProperties() { Super::SynchronizeProperties(); }