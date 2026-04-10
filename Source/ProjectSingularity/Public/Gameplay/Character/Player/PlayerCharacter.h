// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/BaseCharacter.h"
#include "ProjectSingularity/Public/UI/GameHUDSetUp.h"
#include "ProjectSingularity/Public/UI/HUDWidget.h"
#include "PlayerCharacter.generated.h"


#pragma region | Forward Declarations
class UCameraComponent;
class UActionStateFilter;
class UInputAction;
struct FInputActionValue;
class UPlayerConfigDataAsset;
class AWeaponBase;
class UWeaponsDataAsset;
class UStatesDataAsset;
class UStates;
class UPlayerAnimInstance;
#pragma endregion

#pragma region | Delegates
DECLARE_MULTICAST_DELEGATE(FOnInteract);
#pragma endregion

UCLASS()
class PROJECTSINGULARITY_API APlayerCharacter : public ABaseCharacter
{
  GENERATED_BODY()

public:
  FOnInteract m_onInteract;

private:
  UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (DisplayName = "CameraComponent"))
  TObjectPtr<UCameraComponent> m_camera;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Character Actions Filter"))
  TObjectPtr<UActionStateFilter> m_actionsFilterComponent;

  UPROPERTY()
  FTimerHandle m_dashStopTimerHandle;

  UPROPERTY()
  FTimerHandle m_dashResetTimerHandle;

  bool m_bCanDash = true;

#pragma region Inputs

  UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Move Action"))
  TObjectPtr<UInputAction> m_moveAction;

  UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Jump Action"))
  TObjectPtr<UInputAction> m_jumpAction;

  UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Look Action"))
  TObjectPtr<UInputAction> m_lookAction;

  UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Dash Action"))
  TObjectPtr<UInputAction> m_dashAction;

  UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Fire Action"))
  TObjectPtr<UInputAction> m_fireAction;

  UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Change weapon mode Action"))
  TObjectPtr<UInputAction> m_changeWeaponModeAction;

  UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Reload Action"))
  TObjectPtr<UInputAction> m_reloadAction;

  UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "Interact Action"))
  TObjectPtr<UInputAction> m_interactAcion;

#pragma endregion

  UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (DisplayName = "Weapon Class"))
  TSubclassOf<AWeaponBase> m_weaponClass;

  UPROPERTY(EditDefaultsOnly, Category = "Arms")
  USkeletalMeshComponent* m_armsMesh;

  UPROPERTY()
  TObjectPtr<AWeaponBase> m_currentWeapon;

  bool m_bFire = false;

  UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (DisplayName = "Weapon debugs"))
  bool m_bDebugWeapon = false;

  UPROPERTY(EditDefaultsOnly, Category = "Data Asset|Weapon", meta = (DisplayName = "Weapon Data Asset"))
  TObjectPtr<UWeaponsDataAsset> m_weaponDataAsset;

  UPROPERTY(EditDefaultsOnly, Category = "Data Asset|Player", meta = (DisplayName = "Player Config Data Asset"))
  TObjectPtr<UPlayerConfigDataAsset> m_playerDataAsset;

  UPROPERTY(EditDefaultsOnly, Category = "Data Asset|Player", meta = (DisplayName = "Character States Data Asset"))
  TObjectPtr<UStatesDataAsset> m_characterStatesDataAsset;

  TObjectPtr<UPlayerAnimInstance> m_playerAnimInstance;

public:
  USkeletalMeshComponent* GetArmsMesh();

  UFUNCTION(Exec)
  void ShowDebugsWeapon(bool value = true);

  bool GetDebugWeapon();

  TObjectPtr<UPlayerAnimInstance> GetPlayerAnimInstance();

  TObjectPtr<AWeaponBase> GetWeapon() const;

  // Sets default values for this character's properties
  APlayerCharacter();

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  void MoveInternal(const FVector2D& inputVector);

#pragma region Dash Functions

  // Called in DashingState Init()
  UFUNCTION(BlueprintCallable)
  void Dash();

  void DashEnd();

  // Called in DashingState Exit()
  UFUNCTION(BlueprintCallable)
  void StopDash();

  void ResetDash();

#pragma endregion

  void RequestChangeState(const TSubclassOf<UStates> _state);

  virtual void Landed(const FHitResult& Hit) override;

  bool IsGrounded() const;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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

  UFUNCTION()
  void TryToReload();

  UFUNCTION()
  void InteractAction(const FInputActionValue& _Value);
#pragma endregion

  UFUNCTION()
  void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      FVector NormalImpulse, const FHitResult& Hit);
};
