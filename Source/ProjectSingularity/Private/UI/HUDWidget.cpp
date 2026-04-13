#include "ProjectSingularity/Public/UI/HUDWidget.h"

#include "Components/Image.h"
#include "ProjectSingularity/Public/Components/HealthComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeComponent.h"
#include "ProjectSingularity/Public/Components/Hype/PopularityComponent.h"
#include "ProjectSingularity/Public/UI/PopularityWidget.h"
#include "Gameplay/Weapons/WeaponBase.h"

#pragma region | PUBLIC METHODS

void UHUDWidget::BindToHealthComponent(UHealthComponent* _inHealthComp)
{
  if (m_healthComp)
  {
    m_healthComp->OnHealthChanged.RemoveDynamic(this, &UHUDWidget::HandleHealthChanged);
  }

  m_healthComp = _inHealthComp;

  if (m_healthWidget)
  {
    m_healthWidget->BindToHealthComponent(_inHealthComp);
  }

  if (m_healthComp)
  {
    m_healthComp->OnHealthChanged.AddDynamic(this, &UHUDWidget::HandleHealthChanged);
    UpdateVignetteVisual();
  }
}

void UHUDWidget::BindToHypeComponent(UHypeComponent* _inHypeComp)
{
  if (m_hypeWidget)
  {
    m_hypeWidget->BindToHypeComponent(_inHypeComp);
  }
}

void UHUDWidget::BindToPopularityComponent(UPopularityComponent* _inPopularityComp)
{
  if (m_popularityWidget)
  {
    m_popularityWidget->BindToPopularityComponent(_inPopularityComp);
  }
}

void UHUDWidget::BindToWeapon(AWeaponBase* _inWeapon)
{
  if (m_ammoWidget)
  {
    m_ammoWidget->BindToWeapon(_inWeapon);
  }
}

#pragma endregion

#pragma region | PROTECTED METHODS

void UHUDWidget::NativeDestruct()
{
  if (m_healthComp)
  {
    m_healthComp->OnHealthChanged.RemoveDynamic(this, &UHUDWidget::HandleHealthChanged);
  }

  Super::NativeDestruct();
}

void UHUDWidget::NativeTick(const FGeometry& _myGeometry, float _inDeltaTime)
{
  Super::NativeTick(_myGeometry, _inDeltaTime);

  if (m_bVignetteFadingIn)
  {
    if (m_vignetteFadeInTime <= 0.0f)
    {
      m_currentVignetteOpacity = 1.0f;
      m_bVignetteFadingIn = false;
      m_bVignetteHolding = true;
      m_vignetteHoldTimer = m_vignetteHoldTime;
    }
    else
    {
      m_currentVignetteOpacity += _inDeltaTime / m_vignetteFadeInTime;

      if (m_currentVignetteOpacity >= 1.0f)
      {
        m_currentVignetteOpacity = 1.0f;
        m_bVignetteFadingIn = false;
        m_bVignetteHolding = true;
        m_vignetteHoldTimer = m_vignetteHoldTime;
      }
    }

    UpdateVignetteVisual();
    return;
  }

  if (m_bVignetteHolding)
  {
    m_vignetteHoldTimer -= _inDeltaTime;

    if (m_vignetteHoldTimer <= 0.0f)
    {
      m_bVignetteHolding = false;
      m_bVignetteFadingOut = true;
    }

    return;
  }

  if (m_bVignetteFadingOut)
  {
    if (m_vignetteFadeOutTime <= 0.0f)
    {
      m_currentVignetteOpacity = 0.0f;
      m_bVignetteFadingOut = false;
    }
    else
    {
      m_currentVignetteOpacity -= _inDeltaTime / m_vignetteFadeOutTime;

      if (m_currentVignetteOpacity <= 0.0f)
      {
        m_currentVignetteOpacity = 0.0f;
        m_bVignetteFadingOut = false;
      }
    }

    UpdateVignetteVisual();
  }
}

#pragma endregion

#pragma region | PRIVATE METHODS

void UHUDWidget::HandleHealthChanged(float _current, float _max, float _delta, AActor* _instigatorActor)
{
  if (_delta < 0.0f)
  {
    StartVignetteDamageEffect();
  }
}

void UHUDWidget::StartVignetteDamageEffect()
{
  m_bVignetteFadingIn = true;
  m_bVignetteHolding = false;
  m_bVignetteFadingOut = false;
  m_vignetteHoldTimer = 0.0f;
}

void UHUDWidget::UpdateVignetteVisual()
{
  if (!m_vignetteImage)
  {
    return;
  }

  FLinearColor color = m_vignetteImage->GetColorAndOpacity();
  color.A = FMath::Clamp(m_currentVignetteOpacity, 0.0f, m_vignetteOpacity);
  m_vignetteImage->SetColorAndOpacity(color);
}

#pragma endregion