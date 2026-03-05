// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"


class UProgressBar;
class UTextBlock;


UCLASS()
class PROJECTSINGULARITY_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void BindToHealthComponent(UHealthComponent* InHealthComp);

protected:
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthText;

private:
	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComp;

	UFUNCTION()
	void HandleHealthChanged(float Current, float Max, float Delta, AActor* InstigatorActor);

	void RefreshUI(float Current, float Max);
};