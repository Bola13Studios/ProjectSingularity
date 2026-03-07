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

public:


private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (DisplayName = "CameraComponent"))
	TObjectPtr<UCameraComponent> m_Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Move Action"))
	TObjectPtr<UInputAction> m_MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Jump Action"))
	TObjectPtr<UInputAction> m_JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Look Action"))
	TObjectPtr<UInputAction> m_LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Fire Action"))
	TObjectPtr<UInputAction> m_FireAction;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (DisplayName = "Weapon Class"))
	TSubclassOf<AWeaponBase> m_WeaponClass;

	UPROPERTY()
	TObjectPtr<AWeaponBase> m_CurrentWeapon;

	bool m_bFire = false;

	UPROPERTY(EditDefaultsOnly, Category = "Data Asset|Weapon", meta = (DisplayName = "Weapon Data Asset"))
	TObjectPtr<UWeaponsDataAsset> m_WeaponDataAsset;

	UPROPERTY(EditAnywhere, Category = "Data Asset|Player", meta = (DisplayName = "Player Config Data Asset"))
	TObjectPtr<UPlayerConfigDataAsset> m_PlayerDataAsset;	
};
