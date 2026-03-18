// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatesDataAsset.generated.h"

class UStates;

USTRUCT(BlueprintType)
struct FAvailableStates
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UStates>> availableStates;

};

UCLASS()
class PROJECTSINGULARITY_API UStatesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<UStates>, FAvailableStates> StatesMap;

};
