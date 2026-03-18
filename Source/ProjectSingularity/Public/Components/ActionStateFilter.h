// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionStateFilter.generated.h"

class ABaseCharacter;
class UStates;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSINGULARITY_API UActionStateFilter : public UActorComponent
{
	GENERATED_BODY()

public:	
	UActionStateFilter();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsStateAvailable(TSubclassOf<UStates> _state);

	void SetCurrentState(const TSubclassOf<UStates> _newState);

	UStates* GetCurrentBaseState();

	TSubclassOf<UStates> GetCurrentBaseStateClass();

	/*UPROPERTY(EditAnywhere)
	UStateActionsDataAsset* StatesDataAsset = nullptr;*/


	UPROPERTY()
	TMap<TSubclassOf<UStates>, TObjectPtr<UStates>> m_statesInstancesMap;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(Transient)
	TObjectPtr<ABaseCharacter> m_Character = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UStates> m_currentBaseState = nullptr;

	UPROPERTY()
	TSubclassOf<UStates> m_currentBaseStateClass;

		
};
