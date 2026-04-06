// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoWidget.h"
#include "HealthWidget.h"
#include "HypeWidget.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UImage;
class UHealthComponent;
class UHypeComponent;

UCLASS()
class PROJECTSINGULARITY_API UHUDWidget : public UUserWidget
{
  GENERATED_BODY()

public:
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UHealthWidget* HealthWidget;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UAmmoWidget* AmmoWidget;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UHypeWidget* HypeWidget;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UImage* VignetteImage;

  UFUNCTION(BlueprintCallable)
  void BindToHealthComponent(UHealthComponent* InHealthComp);

  UFUNCTION(BlueprintCallable)
  void BindToHypeComponent(UHypeComponent* InHypeComp);

protected:
  virtual void NativeDestruct() override;
  virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
  UPROPERTY()
  TObjectPtr<UHealthComponent> HealthComp;

  UFUNCTION()
  void HandleHealthChanged(float Current, float Max, float Delta, AActor* InstigatorActor);

  void UpdateVignetteVisual();
  void StartVignetteDamageEffect();

private:
  UPROPERTY(EditAnywhere, Category = "Vignette")
  float VignetteFadeInTime = 0.2f;

  UPROPERTY(EditAnywhere, Category = "Vignette")
  float VignetteHoldTime = 1.0f;

  UPROPERTY(EditAnywhere, Category = "Vignette")
  float VignetteFadeOutTime = 0.6f;

  UPROPERTY(EditAnywhere, Category = "Vignette")
  float VignetteOpacitty = 1.0f;

  float CurrentVignetteOpacity = 0.0f;
  float VignetteHoldTimer = 0.0f;

  bool bVignetteFadingIn = false;
  bool bVignetteHolding = false;
  bool bVignetteFadingOut = false;
};
