// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "States.generated.h"

class ABaseCharacter;

UCLASS()
class PROJECTSINGULARITY_API UStates : public UObject
{
	GENERATED_BODY()

public:
	UStates() {};

	virtual void Init() {};
	virtual void Update(float _DeltaTime) {};
	virtual void Exit() {};

	ABaseCharacter* GetOwner() { return OwnerCharacter; };
	void SetOwner(ABaseCharacter* _OwnerCharacter) { OwnerCharacter = _OwnerCharacter; }

	bool CanUpdateTick = false;

protected:
	UPROPERTY(Transient)
	TObjectPtr<ABaseCharacter> OwnerCharacter = nullptr;
	
};

UCLASS()
class PROJECTSINGULARITY_API UGroundMovementState : public UStates
{
	GENERATED_BODY()

public:

	virtual void Init() {};
	virtual void Update(float _DeltaTime) {};
	virtual void Exit() {};

};
