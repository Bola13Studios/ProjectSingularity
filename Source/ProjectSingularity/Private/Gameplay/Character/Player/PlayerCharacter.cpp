// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Character/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectSingularity/Public/Data/DataAsset/PlayerConfigDataAsset.h"
#include "Gameplay/Weapons/WeaponBase.h"
#include "Gameplay/Weapons/WeaponsDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/ActionStateFilter.h"

APlayerCharacter::APlayerCharacter():
	ABaseCharacter()
{
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	if (IsValid(m_Camera))
	{
		m_Camera->SetupAttachment(RootComponent);
		m_Camera->bUsePawnControlRotation = true;
	}

	m_ArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMesh"));
	m_ArmsMesh->SetupAttachment(GetCapsuleComponent());
	m_ArmsMesh->CastShadow = false;
	m_ArmsMesh->SetupAttachment(m_Camera);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCharacterMovementComponent* charMoveComp = GetCharacterMovement();
	if (IsValid(m_PlayerDataAsset) && IsValid(charMoveComp))
	{
		charMoveComp->MaxWalkSpeed = m_PlayerDataAsset->maxWalkSpeed;
		charMoveComp->JumpZVelocity = m_PlayerDataAsset->jumpZVelocity;
		charMoveComp->AirControl = m_PlayerDataAsset->airControl;
		charMoveComp->GravityScale = m_PlayerDataAsset->gravityScale;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;

	m_CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(m_WeaponClass, spawnParams);

	if (IsValid(m_CurrentWeapon))
	{
		m_CurrentWeapon->AttachToComponent(m_ArmsMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("HandGrip_R")); //Temp bone name
		m_CurrentWeapon->SetWeaponData(m_WeaponDataAsset->weaponsData[0]); //Just for now
	}

	if (UCapsuleComponent* capsuleComp = GetCapsuleComponent())
	{
		capsuleComp->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnComponentHit);
	}

}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bFire && m_CurrentWeapon)
	{
		m_bFire = m_CurrentWeapon->Fire();
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedInputComponent->BindAction(m_MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveAction);
		enhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::JumpAction);
		enhancedInputComponent->BindAction(m_LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookAction);
		enhancedInputComponent->BindAction(m_DashAction, ETriggerEvent::Triggered, this, &APlayerCharacter::DashAction);
		enhancedInputComponent->BindAction(m_FireAction, ETriggerEvent::Started, this, &APlayerCharacter::StartFireAction);
		enhancedInputComponent->BindAction(m_FireAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopFireAction);
		enhancedInputComponent->BindAction(m_ChangeWeaponModeAction, ETriggerEvent::Started, this, &APlayerCharacter::ChangeWeaponMode);
		enhancedInputComponent->BindAction(m_ReloadAction, ETriggerEvent::Started, this, &APlayerCharacter::TryToReload);
	}
}

void APlayerCharacter::MoveAction(const FInputActionValue& _inputValue)
{
	FVector2D inputVector = _inputValue.Get<FVector2D>();
	if (IsValid(Controller) && !m_bIsDashing)
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);

		const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(forwardDirection, inputVector.Y);
		AddMovementInput(rightDirection, inputVector.X);
	}
}

void APlayerCharacter::JumpAction()
{
	Jump();
}

void APlayerCharacter::LookAction(const FInputActionValue& _inputValue)
{
	FVector2D inputVector = _inputValue.Get<FVector2D>();
	if (IsValid(Controller))
	{
		AddControllerYawInput(inputVector.X);
		AddControllerPitchInput(inputVector.Y);
	}
}

void APlayerCharacter::StartFireAction(const FInputActionValue& Value)
{
	m_bFire = true;
}

void APlayerCharacter::StopFireAction()
{
	m_bFire = false;
}

void APlayerCharacter::ChangeWeaponMode()
{
	m_CurrentWeapon->TryToChangeMode();
}

void APlayerCharacter::TryToReload()
{
	m_CurrentWeapon->TryToReload();
}

void APlayerCharacter::DashAction()
{
	if (IsValid(m_PlayerDataAsset) && IsValid(m_Camera) && m_bCanDash)
	{
		FVector dashDirection = GetVelocity() * FVector(1, 1, 0);
		dashDirection = dashDirection.IsNearlyZero() ? m_Camera->GetForwardVector() : GetLastMovementInputVector().GetSafeNormal();
		Dash(dashDirection, m_PlayerDataAsset->dashDistance, m_PlayerDataAsset->dashTime);
	}
}

void APlayerCharacter::Dash(const FVector& _direction, float _distance, float _time)
{
	if ((_direction.IsNearlyZero()) || (_distance <= 0.f) || (_time <= 0.f) || !m_bCanDash)
	{
		return;
	}

	FVector dashVelocity = _direction.GetSafeNormal() * (_distance / _time);
	dashVelocity.Z = 0.;

	m_bIsDashing = true;
	m_bCanDash = false;
	UCharacterMovementComponent* charMoveComp = GetCharacterMovement();

	charMoveComp->GravityScale = 0.f;
	charMoveComp->GroundFriction = 0.f;

	LaunchCharacter(dashVelocity, true, true);

	GetWorldTimerManager().SetTimer(m_DashStopTimerHandle, this, &APlayerCharacter::StopDash, _time);
}

void APlayerCharacter::StopDash()
{
	UCharacterMovementComponent* charMoveComp = GetCharacterMovement();
	m_bIsDashing = false;

	if (IsValid(m_PlayerDataAsset) && IsValid(charMoveComp))
	{
		charMoveComp->GravityScale = m_PlayerDataAsset->gravityScale;
		charMoveComp->GroundFriction = m_PlayerDataAsset->groundFriction;
		GetWorldTimerManager().SetTimer(m_DashResetTimerHandle, this, &APlayerCharacter::ResetDash, m_PlayerDataAsset->dashCooldown);
	}
	else
	{
		ResetDash();
	}
}

void APlayerCharacter::ResetDash()
{
	m_bCanDash = true;
}

void APlayerCharacter::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (m_bIsDashing)
	{
		GetWorldTimerManager().ClearTimer(m_DashStopTimerHandle);
		StopDash();
	}
}

USkeletalMeshComponent* APlayerCharacter::GetArmsMesh()
{
	return m_ArmsMesh;
}

void APlayerCharacter::ShowDebugsWeapon(bool value)
{
	m_bDebugWeapon = value;
}

bool APlayerCharacter::GetDebugWeapon()
{
	return m_bDebugWeapon;
}
