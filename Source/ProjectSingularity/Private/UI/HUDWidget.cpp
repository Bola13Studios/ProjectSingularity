// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectSingularity/Public/UI/HUDWidget.h"

#include "Components/Image.h"
#include "Components/HealthComponent.h"
#include "Components/Hype/HypeComponent.h"

void UHUDWidget::BindToHealthComponent(UHealthComponent* InHealthComp)
{
  if (HealthComp)
  {
    HealthComp->OnHealthChanged.RemoveDynamic(this, &UHUDWidget::HandleHealthChanged);
  }

  HealthComp = InHealthComp;

  if (HealthWidget)
  {
    HealthWidget->BindToHealthComponent(InHealthComp);
  }

  if (HealthComp)
  {
    HealthComp->OnHealthChanged.AddDynamic(this, &UHUDWidget::HandleHealthChanged);
    UpdateVignetteVisual();
  }
}

void UHUDWidget::BindToHypeComponent(UHypeComponent* InHypeComp)
{
  if (HypeWidget)
  {
    HypeWidget->BindToHypeComponent(InHypeComp);
  }
}

void UHUDWidget::NativeDestruct()
{
  if (HealthComp)
  {
    HealthComp->OnHealthChanged.RemoveDynamic(this, &UHUDWidget::HandleHealthChanged);
  }

  Super::NativeDestruct();
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
  Super::NativeTick(MyGeometry, InDeltaTime);

  if (bVignetteFadingIn)
  {
    if (VignetteFadeInTime <= 0.0f)
    {
      CurrentVignetteOpacity = 1.0f;
      bVignetteFadingIn = false;
      bVignetteHolding = true;
      VignetteHoldTimer = VignetteHoldTime;
    }
    else
    {
      CurrentVignetteOpacity += InDeltaTime / VignetteFadeInTime;

      if (CurrentVignetteOpacity >= 1.0f)
      {
        CurrentVignetteOpacity = 1.0f;
        bVignetteFadingIn = false;
        bVignetteHolding = true;
        VignetteHoldTimer = VignetteHoldTime;
      }
    }

    UpdateVignetteVisual();
    return;
  }

  if (bVignetteHolding)
  {
    VignetteHoldTimer -= InDeltaTime;

    if (VignetteHoldTimer <= 0.0f)
    {
      bVignetteHolding = false;
      bVignetteFadingOut = true;
    }

    return;
  }

  if (bVignetteFadingOut)
  {
    if (VignetteFadeOutTime <= 0.0f)
    {
      CurrentVignetteOpacity = 0.0f;
      bVignetteFadingOut = false;
    }
    else
    {
      CurrentVignetteOpacity -= InDeltaTime / VignetteFadeOutTime;

      if (CurrentVignetteOpacity <= 0.0f)
      {
        CurrentVignetteOpacity = 0.0f;
        bVignetteFadingOut = false;
      }
    }

    UpdateVignetteVisual();
  }
}

void UHUDWidget::HandleHealthChanged(float Current, float Max, float Delta, AActor* InstigatorActor)
{
  if (Delta < 0.0f)
  {
    StartVignetteDamageEffect();
  }
}

void UHUDWidget::StartVignetteDamageEffect()
{
  bVignetteFadingIn = true;
  bVignetteHolding = false;
  bVignetteFadingOut = false;
  VignetteHoldTimer = 0.0f;
}

void UHUDWidget::UpdateVignetteVisual()
{
  if (!VignetteImage)
  {
    return;
  }

  FLinearColor Color = VignetteImage->GetColorAndOpacity();
  Color.A = FMath::Clamp(CurrentVignetteOpacity, 0.0f, VignetteOpacitty);
  VignetteImage->SetColorAndOpacity(Color);
}
