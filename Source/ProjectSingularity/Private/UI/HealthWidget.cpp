#include "ProjectSingularity/Public/UI/HealthWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#pragma region | PUBLIC METHODS

void UHealthWidget::BindToHealthComponent(UHealthComponent* _inHealthComp)
{
  if (m_healthComp)
  {
    m_healthComp->OnHealthChanged.RemoveDynamic(this, &UHealthWidget::HandleHealthChanged);
  }

  m_healthComp = _inHealthComp;

  if (m_healthComp)
  {
    m_healthComp->OnHealthChanged.AddDynamic(this, &UHealthWidget::HandleHealthChanged);
    RefreshUI(m_healthComp->GetHealth(), m_healthComp->GetMaxHealth());
  }
}

#pragma endregion

#pragma region | PROTECTED METHODS

void UHealthWidget::NativeDestruct()
{
  if (m_healthComp)
  {
    m_healthComp->OnHealthChanged.RemoveDynamic(this, &UHealthWidget::HandleHealthChanged);
  }

  Super::NativeDestruct();
}

#pragma endregion

#pragma region | PRIVATE METHODS

void UHealthWidget::HandleHealthChanged(float _current, float _max, float _delta, AActor* _instigatorActor)
{
  // Play damage animation only when health is reduced
  if (_delta < 0.0f)
  {
    if (m_hpAnim)
    {
      PlayAnimation(m_hpAnim);
    }
  }

  RefreshUI(_current, _max);
}

void UHealthWidget::RefreshUI(float _current, float _max)
{
  const float percent = (_max > 0.0f) ? (_current / _max) : 0.0f;

  if (m_healthBar)
  {
    m_healthBar->SetPercent(percent);
  }

  if (m_healthText)
  {
    const int32 currentHealth = FMath::RoundToInt(_current);
    const int32 maxHealth = FMath::RoundToInt(_max);

    m_healthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), currentHealth, maxHealth)));
  }
}

#pragma endregion