/************************************************************************
 * @description: Button widget with built-in text handling and support for resizing through the parent ResizableUserWidget.
 * @author: Jaime Paramo
 * @date: 10/03/2026
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "ProjectSingularity/Public/UI/UMGPalette/Button/CustomButtonWidget.h"

// Components
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UCustomButtonWidget::ShowWidget() {
  Super::ShowWidget();

  ApplyTextSettings();

  // Ensure the button is visible when the widget is shown.
  if (IsValid(m_pButton)) m_pButton->SetVisibility(ESlateVisibility::Visible);
}

void UCustomButtonWidget::HideWidget() {
  Super::HideWidget();

  // Hide button when the widget is hidden.
  if (IsValid(m_pButton)) m_pButton->SetVisibility(ESlateVisibility::Hidden);
  // Always hide the text when the widget is hidden.
  if (IsValid(m_pButtonText)) m_pButtonText->SetVisibility(ESlateVisibility::Hidden);
}

void UCustomButtonWidget::NativeConstruct() {
  Super::NativeConstruct();

  // Apply initial text visibility & content at runtime.
  ApplyTextSettings();
}

void UCustomButtonWidget::SynchronizeProperties() {
  Super::SynchronizeProperties();

  // Ensure updates also apply inside the editor.
  ApplyTextSettings();
}

void UCustomButtonWidget::ApplyTextSettings() {
  if (!IsValid(m_pButtonText)) return;

  // Update the text displayed in the button.
  m_pButtonText->SetText(m_sButtonText);
  // Toggle text visibility depending on the editor flag.
  m_pButtonText->SetVisibility(m_bShowText ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}