/************************************************************************
 * @description: Game-wide manager implemented as a GameInstance Subsystem.
 * @author: Jaime Paramo
 * @date: 17/03/2026
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Systems/SessionData.h"
#include "Utils/StatHelpers.h"
#include "GameManagerSubsystem.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EGameState, eNewGameState);

// Enums
enum class EGameState : uint8;
enum class EStationStates : uint8;

UCLASS()
class PROJECTSINGULARITY_API UGameManagerSubsystem : public UGameInstanceSubsystem
{
  GENERATED_BODY()

public:
#pragma region Delegates
  /**
   * @brief Delegate triggered when the game state changes.
   */
  UPROPERTY(BlueprintAssignable, Category = "Project Singularity|Game State")
  FOnGameStateChanged OnGameStateChanged;
#pragma endregion

private:
  /**
   * @brief The struct that will save the session data of the player
   */
  UPROPERTY()
  FSessionData m_sessionData;

public:
  /**
   * @brief Used to add a value to a numeric stat, given an accessor to it and the value to add
   * @param _worldContext The context object to retrieve the world from.
   * @param _accessor The accessor function to retrieve the stat from the session data.
   * @param _value The value to add to the stat.
   */
  static void AddStat(UObject* _worldContext, const FStatAccessor& _accessor, int32 _value);

  /**
   * @brief Used to add a value to an array stat, given an accessor to it and the value to add
   * @param _worldContext The context object to retrieve the world from.
   * @param _accessor The accessor function to retrieve the array stat from the session data.
   * @param _key The key to identify the specific element in the map.
   * @param _value The value to add to the map stat.
   */
  static void AddMapStat(UObject* _worldContext,
                         const TFunction<TMap<EStationStates, int32>&(FSessionData&)>& _accessor, EStationStates _key,
                         int32 _value = 1);

#pragma region Getters & Setters
  /**
   * @brief Sets the current game state.
   * @param _eNewState The new state to transition to.
   */
  UFUNCTION(BlueprintCallable, Category = "Project Singularity|Game State")
  void SetGameState(EGameState _eNewState);

  /**
   * @brief Returns the current game state.
   * @return The currently active game state.
   */
  UFUNCTION(BlueprintCallable, Category = "Project Singularity|Game State")
  EGameState GetGameState() const;

  /**
   * @brief Return the stored session data
   * @return The TMap holding the data
   */
  const FSessionData& GetAllData() const;

  /**
   * @brief Return the stored session data
   */
  FSessionData& GetSessionData();
#pragma endregion

  template <typename T> void AddStat(T& Field, const T& Value);

  template <typename T> void AddToArray(TArray<T>& Array, const T& Value);

protected:
#pragma region Native Overrides
  /**
   * @brief Initializes the subsystem.
   * @param _rCollection Collection of subsystems available during initialization.
   */
  virtual void Initialize(FSubsystemCollectionBase& _rCollection) override;
#pragma endregion

private:
#pragma region Transitions
  /**
   * @brief Checks whether a transition between two states is valid.
   * @param _eFrom Current state.
   * @param _eTo Desired next state.
   * @return True if the transition is allowed, false otherwise.
   */
  bool CanTransition(EGameState _eFrom, EGameState _eTo) const;

  /**
   * @brief Defines valid transitions between game states.
   */
  TMap<EGameState, TArray<EGameState>> m_mValidTransitions;
#pragma endregion

#pragma region State
  /**
   * @brief Current active game state.
   */
  UPROPERTY()
  EGameState m_eCurrentGameState;
#pragma endregion
};

template <typename T> void UGameManagerSubsystem::AddStat(T& Field, const T& Value)
{
  Field += Value;
}

template <typename T> void UGameManagerSubsystem::AddToArray(TArray<T>& Array, const T& Value)
{
  Array.Add(Value);
}
