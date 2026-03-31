/************************************************************************
 * @description: Base class for weapons
 * @author: Rubén Santos
 * @date: 01/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"
#include "Utils/State Machine/States.h"
#include "WeaponBase.generated.h"

class UActionStateFilter;
class UStatesDataAsset;
class APlayerCharacter;
class UBaseAnimInstance;
class UHealthComponent;

#pragma region Enums

UENUM(BlueprintType)
enum class EFireMode : uint8
{
  None UMETA(DisplayName = "None"),
  SemiAuto UMETA(DisplayName = "Semi Automatic"),
  FullAuto UMETA(DisplayName = "Full Automatic"),
  Burst UMETA(DisplayName = "Burst")
};

UENUM(BlueprintType)
enum class EWeaponMode : uint8
{
  ShortDistance UMETA(DisplayName = "Short Distance"),
  LongDistance UMETA(DisplayName = "Long Distance")
};

#pragma endregion

#pragma region Structs

// Weapon mode settings
USTRUCT(BlueprintType)
struct FWeaponModeData
{
  GENERATED_BODY()

public:
  // For now is just a tag
  UPROPERTY(EditAnywhere)
  EWeaponMode weaponMode = EWeaponMode::ShortDistance;

  // Fire mode
  UPROPERTY(EditAnywhere)
  EFireMode fireMode = EFireMode::None;

  // Max bullet penetration
  UPROPERTY(EditAnywhere)
  int bulletPenetration = 1;

  // Number if bullets per shot
  UPROPERTY(EditAnywhere)
  int bulletsPerShot = 1;

  // Max angle of bullet spread
  UPROPERTY(EditAnywhere)
  float bulletSpreadAngle = 0;

  // TO DO
  UPROPERTY(EditAnywhere)
  int bulletDamage = 25;

  // Max bullet distance
  UPROPERTY(EditAnywhere)
  float bulletDistance = 5000.f;

  // TO DO
  UPROPERTY(EditAnywhere, meta = (EditCondition = "FireMode == EFireMode::Burst", EditConditionHides))
  int burstBulletsAmount = 3;

  // Rounds Per Minute
  UPROPERTY(EditAnywhere)
  float fireRateRPM = 100.f;

  // Max ammo in magazines
  UPROPERTY(EditAnywhere)
  int maxAmmoInMag = 30;
};

// Weapon general data
USTRUCT(BlueprintType)
struct FWeaponData
{
  GENERATED_BODY()

public:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  int ID;

  // The name for the weapon
  UPROPERTY(EditAnywhere)
  FString name;

  // The mesh for the weapon
  UPROPERTY(EditAnywhere)
  USkeletalMesh* skeletalMesh;

  // The animation blueprint class for the weapon
  UPROPERTY(EditAnywhere)
  TSubclassOf<UAnimInstance> weaponAnimInstance;

  // Weapon anim montage
  UPROPERTY(EditAnywhere)
  UAnimMontage* weaponAnimMontage;

  // Arms anim montage
  UPROPERTY(EditAnywhere)
  UAnimMontage* armsAnimMontage;

  // The states data for the weapon
  UPROPERTY(EditAnywhere)
  TObjectPtr<UStatesDataAsset> statesDataAsset = nullptr;

  // Max ammo in the shared reserve
  UPROPERTY(EditAnywhere)
  int maxAmmoInReser = 90;

  // First mode data
  UPROPERTY(EditAnywhere)
  FWeaponModeData firstMode;

  // Second mode data
  UPROPERTY(EditAnywhere)
  FWeaponModeData secondMode;
};

// Weapon mode variables in the game
USTRUCT()
struct FWeaponModeState
{
  GENERATED_BODY()

  void Initialize(const FWeaponModeData& InData)
  {
    m_modeData = InData;

    currentAmmoInMag = InData.maxAmmoInMag;
    bulletDmg = InData.bulletDamage;
  }

  const FWeaponModeData& GetModeData() const
  {
    return m_modeData;
  }

private:
  FWeaponModeData m_modeData;

public:
  int currentAmmoInMag = 0;
  int bulletDmg = 0;
  int extraBulletDmg = 0;
};

#pragma endregion

UCLASS()
class PROJECTSINGULARITY_API AWeaponBase : public AActor
{
  GENERATED_BODY()

public:
  AWeaponBase();

protected:
  virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
  USkeletalMeshComponent* weaponMesh;

public:
  virtual void Tick(float DeltaTime) override;

  const void SetWeaponData(FWeaponData weaponData);

  UFUNCTION()
  virtual bool Fire();

  virtual void TryToReload();

  virtual void Reload();

  virtual void OnReloadFinished();

  virtual void TryToChangeMode();

  virtual void ChangeMode();

  virtual void OnChangeModeFinished();

  void PlayAnimation(FName name);

  int GetAmmoInMagazine();

  void AddReserveAmmo(int extraAmmo);

  int GetAmmoInReserve();

  void AddExtraBulletDmg(int extraBulletDmg, bool firstMode);

  void SetExtraBulletDmg(int extraBulletDmg, bool firstMode);

  int GetExtraBulletDmg(bool firstMode);

private:
  UPROPERTY()
  TObjectPtr<UCameraComponent> m_cameraComponent;

  UPROPERTY()
  FWeaponData m_weaponData;

  UPROPERTY()
  FWeaponModeState m_firstMode;

  UPROPERTY()
  FWeaponModeState m_secondMode;

  UPROPERTY()
  UActionStateFilter* m_actionsFilterComponent;

  FWeaponModeState* m_currentWeaponMode;

  float m_timeSinceLastShot = 0;

  TObjectPtr<APlayerCharacter> m_player;

  TObjectPtr<UAnimInstance> m_armsAnimInstance;

  TObjectPtr<UAnimInstance> m_weaponAnimInstance;

  int m_currentAmmoInReser = 0;
};
