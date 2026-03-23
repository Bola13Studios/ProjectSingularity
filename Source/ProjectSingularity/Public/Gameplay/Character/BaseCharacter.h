// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13|Components")
	TObjectPtr<UHealthComponent> m_healthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Enemy Data", meta = (DisplayName = "Data Asset"))
	TObjectPtr<UCharacterConfigDataAsset> m_pCharacterDataAsset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
