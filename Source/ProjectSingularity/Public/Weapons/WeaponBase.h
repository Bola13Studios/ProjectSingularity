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
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	None			UMETA(DisplayName = "None"),
	SemiAuto		UMETA(DisplayName = "Semi Automatic"),
	FullAuto		UMETA(DisplayName = "Full Automatic"),
	Burst			UMETA(DisplayName = "Burst")
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

	//UFUNCTION()
	//virtual void Reload();

private:
	UPROPERTY()
	UCameraComponent* m_cameraComponent;

	UPROPERTY()
	FWeaponData m_weaponData;


	float m_elapsedShootTime;
};
