// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


APlayerCharacter::APlayerCharacter():
	ABaseCharacter()
{
	m_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	if (IsValid(m_camera))
	{
		m_camera->SetupAttachment(RootComponent);
		m_camera->bUsePawnControlRotation = true;
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* pEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		pEnhancedInputComponent->BindAction(m_moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveAction);
		pEnhancedInputComponent->BindAction(m_jumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::JumpAction);
		pEnhancedInputComponent->BindAction(m_lookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookAction);
		pEnhancedInputComponent->BindAction(m_runAction, ETriggerEvent::Triggered, this, &APlayerCharacter::RunStartAction);
		pEnhancedInputComponent->BindAction(m_runAction, ETriggerEvent::Completed, this, &APlayerCharacter::RunEndAction);
	}
}

void APlayerCharacter::MoveAction(const FInputActionValue& _inputValue)
{
	FVector2D inputVector = _inputValue.Get<FVector2D>();
	if (IsValid(Controller))
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);

		const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(forwardDirection, inputVector.Y);
		AddMovementInput(rightDirection, inputVector.X);
	}
}

void APlayerCharacter::JumpAction(const FInputActionValue& _inputValue)
{
	ACharacter::Jump();
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

void APlayerCharacter::RunStartAction(const FInputActionValue& _inputValue)
{
	if (UCharacterMovementComponent* charMoveComp = GetCharacterMovement())
	{
		charMoveComp->MaxWalkSpeed = 1200;
	}
}

void APlayerCharacter::RunEndAction(const FInputActionValue& _inputValue)
{
	if (UCharacterMovementComponent* charMoveComp = GetCharacterMovement())
	{
		charMoveComp->MaxWalkSpeed = 800;
	}
}
