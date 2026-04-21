/************************************************************************
 * @description: Handles the HUD widget
 * @author: Serra
 * @date: 20/03/2026
 * @edited_by:
 ************************************************************************/
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
class UPopularityWidget;
class UPopularityComponent;

UCLASS()
class PROJECTSINGULARITY_API UHUDWidget : public UUserWidget
{
  GENERATED_BODY()

#pragma region | Variables

public:
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UHealthWidget* HealthWidget;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UAmmoWidget* AmmoWidget;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UHypeWidget* HypeWidget;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UPopularityWidget* popularityWidget;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UImage* VignetteImage;

private:
  /**
   * @brief Stores a TObjectPtr to UHealthComponent
   */
  UPROPERTY()
  TObjectPtr<UHealthComponent> HealthComp;

  /**
   * @brief Fade-in duration for the vignette effect, in seconds
   */
  UPROPERTY(EditAnywhere, Category = "Vignette")
  float VignetteFadeInTime = 0.2f;

  /**
   * @brief How long the vignette effect is held, in seconds
   */
  UPROPERTY(EditAnywhere, Category = "Vignette")
  float VignetteHoldTime = 1.0f;

  /**
   * @brief Vignette fade-out duration in seconds
   */
  UPROPERTY(EditAnywhere, Category = "Vignette")
  float VignetteFadeOutTime = 0.6f;

  /**
   * @brief Editable float property that controls the vignette opacity
   */
  UPROPERTY(EditAnywhere, Category = "Vignette")
  float VignetteOpacitty = 1.0f;

  /**
   * @brief Stores the current vignette opacity
   */
  float CurrentVignetteOpacity = 0.0f;

  /**
   * @brief Stores the current hold timer for a vignette effect
   */
  float VignetteHoldTimer = 0.0f;

  /**
   * @brief Flag indicating whether the vignette effect is currently fading in
   */
  bool bVignetteFadingIn = false;

  /**
   * @brief Boolean flag indicating whether a vignette is currently being held
   */
  bool bVignetteHolding = false;

  /**
   * @brief Flag indicating whether the vignette effect is currently fading out
   */
  bool bVignetteFadingOut = false;

#pragma endregion

public:
  /**
   * @brief Binds this object to the specified HealthComponent
   * @param InHealthComp The UHealthComponent instance to bind to
   */
  UFUNCTION(BlueprintCallable)
  void BindToHealthComponent(UHealthComponent* InHealthComp);

  /**
   * @brief Binds this object to the specified HypeComponent
   * @param InHypeComp Pointer to the UHypeComponent to bind to
   */
  UFUNCTION(BlueprintCallable)
  void BindToHypeComponent(UHypeComponent* InHypeComp);

  /**
   * @brief Binds this object to the specified PopularityComponent
   * @param _inPopularityComp Pointer to the UPopularityComponent to bind to
   */
  UFUNCTION(BlueprintCallable)
  void BindToPopularityComponent(UPopularityComponent* _inPopularityComp);

protected:
  /**
   * @brief Performs native cleanup for the object and overrides the base class implementation.
   */
  virtual void NativeDestruct() override;

  /**
   * @brief Called every frame to update the widget. Override to implement per-frame behavior such as animations, state
   * updates, or custom layout-related logic.
   * @param MyGeometry The widget's current layout geometry (size, position, and transform) for this tick.
   * @param InDeltaTime Time elapsed since the last tick, in seconds.
   */
  virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
  /**
   * @brief Called when an actor's health value changes
   * @param Current The actor's current health after the change
   * @param Max The actor's maximum possible health
   * @param Delta The amount the health changed (positive for healing, negative for damage)
   * @param InstigatorActor Pointer to the actor that caused the change
   */
  UFUNCTION()
  void HandleHealthChanged(float Current, float Max, float Delta, AActor* InstigatorActor);

  /**
   * @brief Updates or refreshes the vignette visual effect
   */
  void UpdateVignetteVisual();

  /**
   * @brief Starts the vignette damage visual effect
   */
  void StartVignetteDamageEffect();
};
