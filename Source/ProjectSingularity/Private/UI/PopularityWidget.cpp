#include "UI/PopularityWidget.h"
#include "Components/Hype/PopularityComponent.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

void UPopularityWidget::BindToPopularityComponent(UPopularityComponent* _inPopularityComp)
{
  if (_inPopularityComp == m_popularityComponent) return;

  m_popularityComponent = _inPopularityComp;

  m_popularityComponent->OnPopularityChanged.AddDynamic(this, &UPopularityWidget::RefreshUI);

  RefreshUI();
}

void UPopularityWidget::NativeDestruct()
{
  if (m_popularityComponent)
  {
    m_popularityComponent->OnPopularityChanged.RemoveAll(this);
  }

  Super::NativeDestruct();
}

void UPopularityWidget::RefreshUI()
{
  if (!m_popularityComponent) return;

  if (m_multiplierText)
  {
    float multiplier = m_popularityComponent->GetMultiplier();

    m_multiplierText->SetText(FText::FromString(FString::Printf(TEXT("x%.1f"), multiplier)));
  }

  if (m_popularityBar)
  {
    float normalized = m_popularityComponent->GetNormalizedPopularity();

    m_popularityBar->SetPercent(normalized);
  }
}
