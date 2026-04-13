/************************************************************************
 * @description: Widget used to display player health information in the HUD
 * @author: Serra
 * @date: 10/04/2026
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

#pragma region | FORWARD DECLARATIONS

class UProgressBar;
class UTextBlock;
class UWidgetAnimation;

#pragma endregion

UCLASS()
class PROJECTSINGULARITY_API UHealthWidget : public UUserWidget
{
  GENERATED_BODY()

#pragma region | VARIABLES

protected:
  /**
   * @brief Progress bar used to display health percentage
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UProgressBar> m_healthBar;

  /**
   * @brief Text block used to display current and max health
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_healthText;

private:
  /**
   * @brief Cached health component reference
   */
  UPROPERTY()
  TObjectPtr<UHealthComponent> m_healthComp;

  /**
   * @brief Damage feedback animation played when the player loses health
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_hpAnim;

#pragma endregion

#pragma region | METHODS

public:
  /**
   * @brief Bind this widget to a health component
   * @param _inHealthComp health component to bind
   */
  UFUNCTION(BlueprintCallable)
  void BindToHealthComponent(UHealthComponent* _inHealthComp);

protected:
  /**
   * @brief Called when the widget is being destroyed
   */
  virtual void NativeDestruct() override;

private:
  /**
   * @brief Callback executed when health changes
   * @param _current current health value
   * @param _max max health value
   * @param _delta health variation
   * @param _instigatorActor actor responsible for the change
   */
  UFUNCTION()
  void HandleHealthChanged(float _current, float _max, float _delta, AActor* _instigatorActor);

  /**
   * @brief Refresh the health UI values
   * @param _current current health value
   * @param _max max health value
   */
  void RefreshUI(float _current, float _max);

#pragma endregion
};
