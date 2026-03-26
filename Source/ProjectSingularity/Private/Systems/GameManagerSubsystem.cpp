/************************************************************************
 * @description: Game-wide manager implemented as a GameInstance Subsystem.
 * @author: Jaime Paramo
 * @date: 17/03/2026
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "ProjectSingularity/Public/Systems/GameManagerSubsystem.h"

// Enums
#include "ProjectSingularity/Public/Utils/Types/GameStateEnum.h"

void UGameManagerSubsystem::SetGameState(EGameState _eNewState)
{
  // Avoid redundant transitions.
  if (m_eCurrentGameState == _eNewState) return;

  // Validate transition before applying it.
  if (!CanTransition(m_eCurrentGameState, _eNewState))
  {
    UE_LOG(LogTemp, Warning, TEXT("[GAME_STATE] Invalid GameState transition: %d -> %d"), (uint8)m_eCurrentGameState,
           (uint8)_eNewState);
    return;
  }

  // Update current state.
  m_eCurrentGameState = _eNewState;

  // Notify all subscribed systems about the state change.
  OnGameStateChanged.Broadcast(m_eCurrentGameState);
}

EGameState UGameManagerSubsystem::GetGameState() const
{
  // Return current cached state (read-only access).
  return m_eCurrentGameState;
}

float UGameManagerSubsystem::GetOneStat(FName _statName) const
{ // will return the value if the key has been found
  if (const float* _found = m_sessionData.stats.Find(_statName)) return *_found;
  // or -1 if not found
  return -1.0f;
}

const FSessionData& UGameManagerSubsystem::GetAllData() const
{ // will return the full struct with the saved data
  return m_sessionData;
}

void UGameManagerSubsystem::AddStat(FName _statName, float _value)
{ // will find and add or create and assign the value
  m_sessionData.stats.FindOrAdd(_statName) += _value;
}

void UGameManagerSubsystem::ResetSession()
{ // cleaning the saved TMap
  m_sessionData.stats.Empty();
}

void UGameManagerSubsystem::Initialize(FSubsystemCollectionBase& _rCollection)
{
  Super::Initialize(_rCollection);

  // Set initial game state.
  m_eCurrentGameState = EGameState::PLAYING;

  // Define valid transitions for each state.
  m_mValidTransitions.Add(EGameState::PLAYING, {EGameState::PAUSED, EGameState::GAMEOVER});

  m_mValidTransitions.Add(EGameState::PAUSED, {EGameState::PLAYING});

  // This acts as a terminal state unless explicitly extended.
  m_mValidTransitions.Add(EGameState::GAMEOVER, {});
}

bool UGameManagerSubsystem::CanTransition(EGameState _eFrom, EGameState _eTo) const
{
  // Retrieve allowed transitions for the current state.
  const TArray<EGameState>* lAllowedStates = m_mValidTransitions.Find(_eFrom);

  // If no entry exists, the state has no defined transitions.
  if (!lAllowedStates) return false;

  // Check if the desired state is part of the allowed transitions.
  return lAllowedStates->Contains(_eTo);
}
