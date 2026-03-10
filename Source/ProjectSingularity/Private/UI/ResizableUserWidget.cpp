/************************************************************************
 * @description: UserWidget with built-in support for resizing through a configurable SizeBox.
 * @author: Jaime Paramo
 * @date: 10/03/2026
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "ProjectSingularity/Public/UI/ResizableUserWidget.h"

 // Components
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"

void UResizableUserWidget::ShowWidget() { Super::ShowWidget(); }

void UResizableUserWidget::HideWidget() { Super::HideWidget(); }

FVector2D UResizableUserWidget::GetWidgetSize() const {
  return m_vWidgetSize;
}

void UResizableUserWidget::SetWidgetSize(FVector2D _vNewSize) {
  m_vWidgetSize = _vNewSize;

  if (IsValid(m_pRootSizeBox)) {
    m_pRootSizeBox->SetWidthOverride(_vNewSize.X);
    m_pRootSizeBox->SetHeightOverride(_vNewSize.Y);
  }
}

float UResizableUserWidget::GetWidgetWidth() const {
  return m_vWidgetSize.X;
}

void UResizableUserWidget::SetWidgetWidth(float _fNewWidth) {
  m_vWidgetSize.X = _fNewWidth;

  if (IsValid(m_pRootSizeBox)) m_pRootSizeBox->SetWidthOverride(_fNewWidth);
}

float UResizableUserWidget::GetWidgetHeight() const {
  return m_vWidgetSize.Y;
}

void UResizableUserWidget::SetWidgetHeight(float _fNewHeight) {
  m_vWidgetSize.Y = _fNewHeight;

  if (IsValid(m_pRootSizeBox)) m_pRootSizeBox->SetHeightOverride(_fNewHeight);
}

void UResizableUserWidget::NativeConstruct() {
  // Called at runtime: ensures the widget size and CanvasPanel auto-size are applied when the widget is spawned.
  Super::NativeConstruct();

  SetWidgetSize(m_vWidgetSize);
  UpdateSizeToContent();
}

void UResizableUserWidget::SynchronizeProperties() {
  // Called in-editor: ensures the widget size and CanvasPanel auto-size are applied in editor preview.
  Super::SynchronizeProperties();

  SetWidgetSize(m_vWidgetSize);
  UpdateSizeToContent();
}

void UResizableUserWidget::UpdateSizeToContent() {
  if (!IsValid(m_pRootSizeBox)) return;

  // For CanvasPanelSlot, enable auto-sizing so the SizeBox can drive the final widget size.
  // Other panel types will respect the SizeBox dimensions automatically.
  if (UCanvasPanelSlot* pCanvasSlot = Cast<UCanvasPanelSlot>(m_pRootSizeBox->Slot)) {
    pCanvasSlot->SetAutoSize(true);
  }
}