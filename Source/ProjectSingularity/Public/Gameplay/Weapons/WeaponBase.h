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
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	None			UMETA(DisplayName = "None"),
	SemiAuto		UMETA(DisplayName = "Semi Automatic"),
	FullAuto		UMETA(DisplayName = "Full Automatic"),
	Burst			UMETA(DisplayName = "Burst")
};

UENUM(BlueprintType)
enum class EWeaponMode : uint8
{
	ShortDistance	UMETA(DisplayName = "Short Distance"),
	LongDistance	UMETA(DisplayName = "Long Distance")
};

USTRUCT(BlueprintType)
struct FWeaponModeData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	EWeaponMode weaponMode = EWeaponMode::ShortDistance;

	UPROPERTY(EditAnywhere)
	EFireMode fireMode = EFireMode::None;

	//Max bullet penetration
	UPROPERTY(EditAnywhere)
	int bulletPenetration = 1;
	//Number if bullets per shot
	UPROPERTY(EditAnywhere)
	int bulletsPerShot = 1;
	//Max angle of bullet spread
	UPROPERTY(EditAnywhere)
	float bulletSpreadAngle = 0;
	//TO DO
	UPROPERTY(EditAnywhere)
	int bulletDamage = 25;
	//Max bullet distance
	UPROPERTY(EditAnywhere)
	float bulletDistance = 5000.f;

	//TO DO
	UPROPERTY(EditAnywhere, meta = (EditCondition = "FireMode == EFireMode::Burst", EditConditionHides))
	int burstBulletsAmount = 3;

	//Rounds Per Minute
	UPROPERTY(EditAnywhere)
	float fireRateRPM = 100.f;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int ID;

	UPROPERTY(EditAnywhere)
	FString name;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* skeletalMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> animInstance;

	UPROPERTY(EditAnywhere)
	UAnimMontage* animMontage;

	UPROPERTY(EditAnywhere)
	FWeaponModeData firstMode;

	UPROPERTY(EditAnywhere)
	FWeaponModeData secondMode;

};



UCLASS()
class PROJECTSINGULARITY_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	const void SetWeaponData(FWeaponData weaponData);
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* weaponMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

UFUNCTION()
	virtual bool Fire();

	void ChangeWeaponMode();

	//UFUNCTION()
	//virtual void Reload();

private:
	UPROPERTY()
	UCameraComponent* m_cameraComponent;

	UPROPERTY()
	FWeaponData m_weaponData;

	UPROPERTY()
	FWeaponModeData m_firstMode;

	UPROPERTY()
	FWeaponModeData m_secondMode;

	FWeaponModeData* m_currentWeaponMode;

	float m_elapsedShootTime;
};
