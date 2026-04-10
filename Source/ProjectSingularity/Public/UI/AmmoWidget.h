/************************************************************************
 * @description: Widget used to display ammo information in the HUD
 * @author: Serra
 * @date: 10/04/2026
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/Weapons/WeaponBase.h"
#include "AmmoWidget.generated.h"

class UTextBlock;
class AWeaponBase;

UCLASS()
class PROJECTSINGULARITY_API UAmmoWidget : public UUserWidget
{
  GENERATED_BODY()

#pragma region | VARIABLES

protected:

  /**
   * @brief Text displaying ammo in current magazine
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_magazineAmmoText;

  /**
   * @brief Text displaying shared reserve ammo
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_reserveAmmoText;

  /**
   * @brief Text displaying current active weapon mode label
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_weaponModeText;

private:

  /**
   * @brief Cached weapon reference
   */
  UPROPERTY()
  TObjectPtr<AWeaponBase> m_weapon;

  /**
   * @brief Cached current ammo in active magazine
   */
  int m_currentAmmo = 0;

  /**
   * @brief Cached max ammo in active magazine
   */
  int m_maxAmmo = 0;

  /**
   * @brief Cached reserve ammo
   */
  int m_reserveAmmo = 0;

  /**
   * @brief Cached current active mode
   */
  EWeaponMode m_currentMode = EWeaponMode::ShortDistance;

#pragma endregion

#pragma region | METHODS

public:

  /**
   * @brief Bind widget to weapon
   * @param _weapon weapon to bind
   */
  UFUNCTION(BlueprintCallable)
  void BindToWeapon(AWeaponBase* _weapon);

protected:

  /**
   * @brief Cleanup when widget is destroyed
   */
  virtual void NativeDestruct() override;

private:

  /**
   * @brief Callback when reserve ammo changes
   * @param _currentReserveAmmo current reserve ammo
   */
  UFUNCTION()
  void HandleReserveAmmoChanged(int _currentReserveAmmo);

  /**
   * @brief Callback when first mode ammo changes
   * @param _currentAmmoInMag current ammo in magazine
   * @param _maxAmmoInMag max ammo in magazine
   */
  UFUNCTION()
  void HandleFirstModeAmmoChanged(int _currentAmmoInMag, int _maxAmmoInMag);

  /**
   * @brief Callback when second mode ammo changes
   * @param _currentAmmoInMag current ammo in magazine
   * @param _maxAmmoInMag max ammo in magazine
   */
  UFUNCTION()
  void HandleSecondModeAmmoChanged(int _currentAmmoInMag, int _maxAmmoInMag);

  /**
   * @brief Callback when current weapon mode changes
   * @param _currentMode current active mode
   */
  UFUNCTION()
  void HandleCurrentModeChanged(EWeaponMode _currentMode);

  /**
   * @brief Refresh ammo widget values
   */
  void RefreshUI();

#pragma endregion
};