// Fill out your copyright notice in the Description page of Project Settings.
/************************************************************************
 * @description: This class serves as the base character for all characters in the game, including both player and
 *enemy characters. It provides common functionality and properties that can be shared among different character types.
 *The class includes a health component to manage the character's health and a reference to a character configuration
 *data asset for storing character-specific data.
 * @author: Lucia Fernandez
 * @date: 25/03/2026
 * @edited_by: 
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCharacterConfigDataAsset;
class UHealthComponent;

UCLASS()
class PROJECTSINGULARITY_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Health component to manage the character's health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13|Components")
	TObjectPtr<UHealthComponent> m_healthComponent;

	// Reference to a character configuration data asset for storing character-specific data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Enemy Data", meta = (DisplayName = "Data Asset"))
	TObjectPtr<UCharacterConfigDataAsset> m_pCharacterDataAsset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
