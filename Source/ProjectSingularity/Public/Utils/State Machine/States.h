// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "States.generated.h"

class ABaseCharacter;
class AWeaponBase;

UCLASS()
class PROJECTSINGULARITY_API UStates : public UObject
{
	GENERATED_BODY()

public:
	UStates() {};

	virtual void Init() {};
	virtual void Update(float _DeltaTime) {};
	virtual void Exit() {};

	void SetOwner(AActor* InOwner) { Owner = InOwner; }

	bool CanUpdateTick = false;

protected:
	UPROPERTY(Transient)
	TObjectPtr<AActor> Owner = nullptr;
	
	template<typename T>
	T* GetOwnerAs() const
	{
		return Cast<T>(Owner);
	}
};


#pragma region CharacterStates

UCLASS()
class PROJECTSINGULARITY_API UCharacterBase : public UStates
{
	GENERATED_BODY()

public:

	virtual void Init() {};
	virtual void Update(float _DeltaTime) {};
	virtual void Exit() {};


protected:
	ABaseCharacter* GetCharacter() const;
};



UCLASS()
class PROJECTSINGULARITY_API UGroundMovementState : public UCharacterBase
{
	GENERATED_BODY()

public:

	virtual void Init() {};
	virtual void Update(float _DeltaTime) {};
	virtual void Exit() {};

};
#pragma endregion

#pragma region WeaponStates

UCLASS()
class PROJECTSINGULARITY_API UWeaponBaseState : public UStates
{
	GENERATED_BODY()

public:

	virtual void Init() {};
	virtual void Update(float _DeltaTime) {};
	virtual void Exit() {};

protected:
	AWeaponBase* GetWeapon() const;
};

UCLASS()
class PROJECTSINGULARITY_API UWeaponIdle : public UWeaponBaseState
{
	GENERATED_BODY()

public:

	virtual void Init() {};
	virtual void Update(float _DeltaTime) {};
	virtual void Exit() {};

};

UCLASS()
class PROJECTSINGULARITY_API UWeaponReload : public UWeaponBaseState
{
	GENERATED_BODY()

public:

	virtual void Init();
	virtual void Update(float _DeltaTime);
	virtual void Exit();

};

UCLASS()
class PROJECTSINGULARITY_API UWeaponChangeMode : public UWeaponBaseState
{
	GENERATED_BODY()

public:

	virtual void Init();
	virtual void Update(float _DeltaTime);
	virtual void Exit();

};

#pragma endregion
