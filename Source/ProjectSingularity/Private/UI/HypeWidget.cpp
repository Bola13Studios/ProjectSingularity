#include "ProjectSingularity/Public/UI/HypeWidget.h"

#include "Components/TextBlock.h"
#include "Components/Hype/HypeComponent.h"

#pragma region | PUBLIC METHODS

void UHypeWidget::BindToHypeComponent(UHypeComponent* _inHypeComp)
{
  if (m_hypeComp == _inHypeComp)
  {
    return;
  }

  if (m_hypeComp)
  {
    m_hypeComp->onHypeChanged.RemoveDynamic(this, &UHypeWidget::HandleHypeChanged);
  }

  m_hypeComp = _inHypeComp;

  if (m_hypeComp)
  {
    m_hypeComp->onHypeChanged.AddDynamic(this, &UHypeWidget::HandleHypeChanged);
    RefreshUI(m_hypeComp->GetHype());
  }
}

#pragma endregion

#pragma region | PROTECTED METHODS

void UHypeWidget::NativeDestruct()
{
  if (m_hypeComp)
  {
    m_hypeComp->onHypeChanged.RemoveDynamic(this, &UHypeWidget::HandleHypeChanged);
  }

  Super::NativeDestruct();
}

#pragma endregion

#pragma region | PRIVATE METHODS

void UHypeWidget::HandleHypeChanged(int _currentHype, int _delta)
{
  RefreshUI(_currentHype);
}

void UHypeWidget::RefreshUI(int _currentHype)
{
  if (m_hypeText)
  {
    m_hypeText->SetText(FText::AsNumber(_currentHype));
  }
}

#pragma endregion