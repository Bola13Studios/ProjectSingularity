// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class UInputMappingContext;

UCLASS()
class PROJECTSINGULARITY_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	

public:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Gameplay Input Mapping Context"))
	TObjectPtr<UInputMappingContext> m_GameplayIMC;
};
