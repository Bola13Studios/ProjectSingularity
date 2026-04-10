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
class AWeaponBase;

UCLASS()
class PROJECTSINGULARITY_API UHUDWidget : public UUserWidget
{
  GENERATED_BODY()

#pragma region | VARIABLES

public:

  /**
   * @brief Widget used to display health data
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UHealthWidget* m_healthWidget;

  /**
   * @brief Widget used to display ammo data
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UAmmoWidget* m_ammoWidget;

  /**
   * @brief Widget used to display hype data
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UHypeWidget* m_hypeWidget;

  /**
   * @brief Widget used to display popularity data
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UPopularityWidget* m_popularityWidget;

  /**
   * @brief Fullscreen vignette image used as damage feedback
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UImage* m_vignetteImage;

private:

  /**
   * @brief Cached health component reference
   */
  UPROPERTY()
  TObjectPtr<UHealthComponent> m_healthComp;

  /**
   * @brief Fade-in duration for the vignette effect, in seconds
   */
  UPROPERTY(EditAnywhere, Category = "Vignette")
  float m_vignetteFadeInTime = 0.2f;

  /**
   * @brief How long the vignette effect is held, in seconds
   */
  UPROPERTY(EditAnywhere, Category = "Vignette")
  float m_vignetteHoldTime = 1.0f;

  /**
   * @brief Vignette fade-out duration in seconds
   */
  UPROPERTY(EditAnywhere, Category = "Vignette")
  float m_vignetteFadeOutTime = 0.6f;

  /**
   * @brief Maximum allowed opacity for the vignette effect
   */
  UPROPERTY(EditAnywhere, Category = "Vignette")
  float m_vignetteOpacity = 1.0f;

  /**
   * @brief Current vignette opacity value
   */
  float m_currentVignetteOpacity = 0.0f;

  /**
   * @brief Current hold timer for the vignette effect
   */
  float m_vignetteHoldTimer = 0.0f;

  /**
   * @brief Whether the vignette is currently fading in
   */
  bool m_bVignetteFadingIn = false;

  /**
   * @brief Whether the vignette is currently being held
   */
  bool m_bVignetteHolding = false;

  /**
   * @brief Whether the vignette is currently fading out
   */
  bool m_bVignetteFadingOut = false;

#pragma endregion

#pragma region | METHODS

public:

  /**
   * @brief Bind this HUD to a health component
   * @param _inHealthComp health component to bind
   */
  UFUNCTION(BlueprintCallable)
  void BindToHealthComponent(UHealthComponent* _inHealthComp);

  /**
   * @brief Bind this HUD to a hype component
   * @param _inHypeComp hype component to bind
   */
  UFUNCTION(BlueprintCallable)
  void BindToHypeComponent(UHypeComponent* _inHypeComp);

  /**
   * @brief Bind this HUD to a popularity component
   * @param _inPopularityComp popularity component to bind
   */
  UFUNCTION(BlueprintCallable)
  void BindToPopularityComponent(UPopularityComponent* _inPopularityComp);

  /**
   * @brief Bind this HUD to the current weapon
   * @param _inWeapon current weapon to bind
   */
  UFUNCTION(BlueprintCallable)
  void BindToWeapon(AWeaponBase* _inWeapon);

protected:

  /**
   * @brief Cleanup executed when the widget is destroyed
   */
  virtual void NativeDestruct() override;

  /**
   * @brief Widget tick used for vignette animation updates
   * @param _myGeometry widget geometry
   * @param _inDeltaTime delta time in seconds
   */
  virtual void NativeTick(const FGeometry& _myGeometry, float _inDeltaTime) override;

private:

  /**
   * @brief Callback executed when health changes
   * @param _current current health
   * @param _max max health
   * @param _delta health delta
   * @param _instigatorActor actor causing the change
   */
  UFUNCTION()
  void HandleHealthChanged(float _current, float _max, float _delta, AActor* _instigatorActor);

  /**
   * @brief Update vignette visual opacity
   */
  void UpdateVignetteVisual();

  /**
   * @brief Start vignette damage effect
   */
  void StartVignetteDamageEffect();

#pragma endregion
};