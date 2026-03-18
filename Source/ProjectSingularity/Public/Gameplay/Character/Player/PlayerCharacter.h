// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"


class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UPlayerConfigDataAsset;
class AWeaponBase;
class UWeaponsDataAsset;

UCLASS()
class PROJECTSINGULARITY_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
#pragma region Inputs Functions
	UFUNCTION()
	void MoveAction(const FInputActionValue& _inputValue);

	UFUNCTION()
	void JumpAction();

	UFUNCTION()
	void LookAction(const FInputActionValue& _inputValue);

	UFUNCTION()
	void StartFireAction(const FInputActionValue& Value);

	UFUNCTION()
	void StopFireAction();

	UFUNCTION()
	void DashAction();

	UFUNCTION()
	void ChangeWeaponMode();
#pragma endregion

#pragma region Dash Functions

	UFUNCTION(BlueprintCallable)
	void Dash(const FVector& _Direction, float _Distance, float _Time);

	UFUNCTION(BlueprintCallable)
	void StopDash();

	void ResetDash();

#pragma endregion

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:

	USkeletalMeshComponent* GetArmsMesh();
private:

	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (DisplayName = "CameraComponent"))
	TObjectPtr<UCameraComponent> m_Camera;

	UPROPERTY()
	FTimerHandle m_DashStopTimerHandle;

	UPROPERTY()
	FTimerHandle m_DashResetTimerHandle;

	bool m_bIsDashing = false;

	bool m_bCanDash = true;

#pragma region Inputs

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Move Action"))
	TObjectPtr<UInputAction> m_MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Jump Action"))
	TObjectPtr<UInputAction> m_JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Look Action"))
	TObjectPtr<UInputAction> m_LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Dash Action"))
	TObjectPtr<UInputAction> m_DashAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Fire Action"))
	TObjectPtr<UInputAction> m_FireAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Change weapon mode Action"))
	TObjectPtr<UInputAction> m_ChangeWeaponMode;
#pragma endregion

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (DisplayName = "Weapon Class"))
	TSubclassOf<AWeaponBase> m_WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Arms")
	USkeletalMeshComponent* m_ArmsMesh;

	UPROPERTY()
	TObjectPtr<AWeaponBase> m_CurrentWeapon;

	bool m_bFire = false;

	UPROPERTY(EditDefaultsOnly, Category = "Data Asset|Weapon", meta = (DisplayName = "Weapon Data Asset"))
	TObjectPtr<UWeaponsDataAsset> m_WeaponDataAsset;

	UPROPERTY(EditAnywhere, Category = "Data Asset|Player", meta = (DisplayName = "Player Config Data Asset"))
	TObjectPtr<UPlayerConfigDataAsset> m_PlayerDataAsset;	
};
