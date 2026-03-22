// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoWidget.h"
#include "HealthWidget.h"
#include "HypeWidget.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSINGULARITY_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UHealthWidget* HealthWidget;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAmmoWidget* AmmoWidget;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UHypeWidget* HypeWidget;

	UFUNCTION(BlueprintCallable)
	void BindToHealthComponent(UHealthComponent* InHealthComp);

	UFUNCTION(BlueprintCallable)
	void BindToHypeComponent(UHypeComponent* InHypeComp);
};
