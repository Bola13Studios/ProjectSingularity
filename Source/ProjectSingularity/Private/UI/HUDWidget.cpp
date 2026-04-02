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
    UpdateVignette(HealthComp->GetHealth(), HealthComp->GetMaxHealth());
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
}

void UHUDWidget::HandleHealthChanged(float Current, float Max, float Delta, AActor* InstigatorActor)
{
  UpdateVignette(Current, Max);
}

void UHUDWidget::UpdateVignette(float Current, float Max)
{
  if (!VignetteImage)
  {
    return;
  }

  const float HealthPercent = (Max > 0.0f) ? (Current / Max) : 0.0f;
  const float Threshold = 0.5f;

  float Opacity = 0.0f;

  if (HealthPercent <= Threshold)
  {
    Opacity = (Threshold - HealthPercent) / Threshold;
  }

  Opacity = FMath::Clamp(Opacity, 0.0f, 1.0f);

  FLinearColor Color = VignetteImage->GetColorAndOpacity();
  Color.A = Opacity;
  VignetteImage->SetColorAndOpacity(Color);
}
