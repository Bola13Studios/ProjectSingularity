// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "States.generated.h"

class ABaseCharacter;
class AWeaponBase;
struct FInputActionValue;

UCLASS()
class PROJECTSINGULARITY_API UStates : public UObject
{
  GENERATED_BODY()

public:
  UStates() {};

  virtual void Init() {};
  virtual void Update(float _DeltaTime) {};
  virtual void HandleInput(const FInputActionValue& _inputValue) {};
  virtual void Exit() {};

  void SetOwner(AActor* InOwner)
  {
    Owner = InOwner;
  }

  bool CanUpdateTick = false;

protected:
  UPROPERTY(Transient)
  TObjectPtr<AActor> Owner = nullptr;

  template <typename T> T* GetOwnerAs() const
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
  virtual void Init() override {};
  virtual void Update(float _DeltaTime) override {};
  virtual void HandleInput(const FInputActionValue& _inputValue) override {};
  virtual void Exit() override {};

protected:
  ABaseCharacter* GetCharacter() const;
};

UCLASS()
class PROJECTSINGULARITY_API UGroundMovementState : public UCharacterBase
{
  GENERATED_BODY()

public:
  virtual void Init() override;
  virtual void Update(float _DeltaTime) override;
  virtual void HandleInput(const FInputActionValue& _inputValue) override;
  virtual void Exit() override;
};

UCLASS()
class PROJECTSINGULARITY_API UDashingState : public UCharacterBase
{
  GENERATED_BODY()

public:
  virtual void Init() override;
  virtual void Update(float _DeltaTime) override;
  virtual void Exit() override;
};

UCLASS()
class PROJECTSINGULARITY_API UJumpingState : public UCharacterBase
{
  GENERATED_BODY()

public:
  virtual void Init() override;
  virtual void Update(float _DeltaTime) override;
  virtual void HandleInput(const FInputActionValue& _inputValue) override;
  virtual void Exit() override;
};

UCLASS()
class PROJECTSINGULARITY_API UFallingState : public UCharacterBase
{
  GENERATED_BODY()

public:
  virtual void Init() override;
  virtual void Update(float _DeltaTime) override;
  virtual void HandleInput(const FInputActionValue& _inputValue) override;
  virtual void Exit() override;
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
