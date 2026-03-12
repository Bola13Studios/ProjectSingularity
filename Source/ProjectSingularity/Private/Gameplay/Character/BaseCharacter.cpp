// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectSingularity/Public/Data/CharacterConfigDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectSingularity/Public/Components/HealthComponent.h"
#include "Gameplay/Character/BaseCharacter.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UCharacterMovementComponent* charMoveComp = GetCharacterMovement();
	if (IsValid(m_pCharacterDataAsset) && IsValid(charMoveComp))
	{
		charMoveComp->MaxWalkSpeed = m_pCharacterDataAsset->maxWalkSpeed;
		m_healthComponent->SetHealth(m_pCharacterDataAsset->health);
		m_healthComponent->SetMaxHealth(m_pCharacterDataAsset->maxHealth);

	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

