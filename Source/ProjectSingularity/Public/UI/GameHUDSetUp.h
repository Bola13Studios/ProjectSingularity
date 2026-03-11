// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDWidget.h"
#include "GameFramework/HUD.h"
#include "GameHUDSetUp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSINGULARITY_API AGameHUDSetUp : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	// Runtime State
	UPROPERTY(BlueprintReadWrite)
	TArray<UUserWidget*> GameMenusWidgetList{};

	// Widget Classes (configured in BP)
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UHUDWidget> HUDWidgetClass{};

	// Widget Instances
	UPROPERTY(BlueprintReadWrite)
	UHUDWidget* HUDWidget = nullptr;

protected:
	
	virtual void BeginPlay() override;

private:
	
	UPROPERTY()
	UGameInstance* GameInstance = nullptr;

	UFUNCTION()
	void InitializeAllGameMenus();

	UFUNCTION()
	void TryBindHUDToPawn() const;
};
