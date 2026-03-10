/************************************************************************
 * @description: Base class extending Unreal Engine's UserWidget.
 * @author: Jaime Paramo
 * @date: 10/03/2026
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "ProjectSingularity/Public/UI/CustomUserWidget.h"

void UCustomUserWidget::ShowWidget() {
  // Mark this widget as visible.
  m_bWidgetVisible = true;

  // Propagate visibility to all contained child widgets.
  for (TObjectPtr<UCustomUserWidget> pCustomUserWidget : m_lContainedWidgets) {
    if (IsValid(pCustomUserWidget)) pCustomUserWidget->ShowWidget();
  }
}

void UCustomUserWidget::HideWidget() {
  // Mark this widget as hidden.
  m_bWidgetVisible = false;

  // Propagate the hide state to all contained child widgets.
  for (TObjectPtr<UCustomUserWidget> pCustomUserWidget : m_lContainedWidgets) {
    if (IsValid(pCustomUserWidget)) pCustomUserWidget->HideWidget();
  }
}

bool UCustomUserWidget::IsWidgetVisible() { return m_bWidgetVisible; }

void UCustomUserWidget::NativeConstruct() { Super::NativeConstruct(); }