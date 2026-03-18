/************************************************************************
 * @description: Game-wide manager implemented as a GameInstance Subsystem.
 * @author: Jaime Paramo
 * @date: 17/03/2026
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameManagerSubsystem.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EGameState, eNewGameState);

// Enums
enum class EGameState : uint8;

UCLASS()
class PROJECTSINGULARITY_API UGameManagerSubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
#pragma region Delegates
  /**
   * @brief Delegate triggered when the game state changes.
   */
  UPROPERTY(BlueprintAssignable, Category = "Project Singularity|Game State")
  FOnGameStateChanged OnGameStateChanged;
#pragma endregion

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
#pragma endregion

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