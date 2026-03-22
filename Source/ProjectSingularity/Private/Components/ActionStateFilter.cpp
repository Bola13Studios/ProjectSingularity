#include "Components/ActionStateFilter.h"
#include "Data/DataAsset/StatesDataAsset.h"
#include "Utils/State Machine/States.h"

UActionStateFilter::UActionStateFilter()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UActionStateFilter::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UActionStateFilter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (StatesDataAsset == NULL)
	{
		return;
	}

	if (!m_currentBaseState && m_currentBaseState->CanUpdateTick)
	{
		m_currentBaseState->Update(DeltaTime);
	}

}

bool UActionStateFilter::IsStateAvailable(const TSubclassOf<UStates> _state)
{
	bool StateAvailable = false;
	if (!IsValid(StatesDataAsset))
	{
		return false;
	}

	for (const auto& Pair : StatesDataAsset->StatesMap)
	{
		if (Pair.Key == m_currentBaseStateClass)
		{
			for (const auto& AvailableState : Pair.Value.availableStates)
			{
				if (AvailableState == _state)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void UActionStateFilter::SetCurrentState(TSubclassOf<UStates> _newState)
{
	if (!m_statesInstancesMap.Contains(_newState))
	{
		return;
	}

	UStates* NewState = *m_statesInstancesMap.Find(_newState);

	if (m_currentBaseState != nullptr && NewState != nullptr)
	{
		if (IsStateAvailable(_newState))
		{
			m_currentBaseState->Exit();

			m_currentBaseState = NewState;
			m_currentBaseStateClass = _newState;

			NewState->Init();
		}
	}
}

void UActionStateFilter::InitializeFilter(AActor* _owner, TObjectPtr<UStatesDataAsset> _statesDataAsset, const TSubclassOf<UStates> _state)
{
	StatesDataAsset = _statesDataAsset;

	if (!IsValid(StatesDataAsset))
	{
		return;
	}

	m_Owner = _owner;

	// Pair is of type TSubclassOf<UBaseState>, FAvailableStates>
	for (const auto& Pair : StatesDataAsset->StatesMap)
	{
		if (UStates* NewStateInstance = NewObject<UStates>(this, Pair.Key))
		{
			m_statesInstancesMap.Add(Pair.Key, NewStateInstance);
			NewStateInstance->SetOwner(m_Owner);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create instance of state class."));
		}
	}

	m_currentBaseState = *m_statesInstancesMap.Find(_state);
	m_currentBaseStateClass = _state;

	m_currentBaseState->Init();
}

