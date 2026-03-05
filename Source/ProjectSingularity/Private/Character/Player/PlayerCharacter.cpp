// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectSingularity/Public/Data/PlayerConfigDataAsset.h"


APlayerCharacter::APlayerCharacter():
	ABaseCharacter()
{
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	if (IsValid(m_Camera))
	{
		m_Camera->SetupAttachment(RootComponent);
		m_Camera->bUsePawnControlRotation = true;
	}
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
		pEnhancedInputComponent->BindAction(m_MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveAction);
		pEnhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::JumpAction);
		pEnhancedInputComponent->BindAction(m_LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookAction);
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
