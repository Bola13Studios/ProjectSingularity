#include "ProjectSingularity/Public/UI/PopularityWidget.h"
#include "ProjectSingularity/Public/Components/Hype/PopularityComponent.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

void UPopularityWidget::BindToPopularityComponent(UPopularityComponent* _inPopularityComp)
{
  if (_inPopularityComp == m_popularityComponent) return;

  m_popularityComponent = _inPopularityComp;

  m_popularityComponent->onPopularityChanged.AddDynamic(this, &UPopularityWidget::RefreshUI);

  RefreshUI();
}

void UPopularityWidget::NativeConstruct()
{
  // setting the default values for the popularity bar and multiplier text
  if (m_multiplierText) m_multiplierText->SetText(FText::FromString(FString::Printf(TEXT("x1.0"))));
  if (m_popularityBar) m_popularityBar->SetPercent(0.0f);
}

void UPopularityWidget::NativeDestruct()
{
  if (IsValid(m_popularityComponent)) m_popularityComponent->onPopularityChanged.RemoveAll(this);

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
