// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HypeWidget.generated.h"

class UTextBlock;
class UHypeComponent;

UCLASS()
class PROJECTSINGULARITY_API UHypeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void BindToHypeComponent(UHypeComponent* InHypeComp);

protected:
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HypeText;

private:
	UPROPERTY()
	TObjectPtr<UHypeComponent> HypeComp;

	UFUNCTION()
	void HandleHypeChanged(int CurrentHype, int Delta);

	void RefreshUI(int CurrentHype);
};