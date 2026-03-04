// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"


class UCameraComponent;
class UInputAction;
struct FInputActionValue;

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

	void MoveAction(const FInputActionValue& _inputValue);
	void JumpAction(const FInputActionValue& _inputValue);
	void LookAction(const FInputActionValue& _inputValue);
	void RunStartAction(const FInputActionValue& _inputValue);
	void RunEndAction(const FInputActionValue& _inputValue);


private:
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "CameraComponent"))
	TObjectPtr<UCameraComponent> m_camera;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Move Action"))
	TObjectPtr<UInputAction> m_moveAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Jump Action"))
	TObjectPtr<UInputAction> m_jumpAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Look Action"))
	TObjectPtr<UInputAction> m_lookAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Run Action"))
	TObjectPtr<UInputAction> m_runAction;
	
};
