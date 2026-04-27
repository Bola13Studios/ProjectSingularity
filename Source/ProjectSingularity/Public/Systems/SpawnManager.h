/************************************************************************
 * @description: This class defines the spawner manager for the spawn points
 * @author: Josephine Esposito
 * @date: 24/04/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "ProjectSingularity/Public/Gameplay/Spawner/EnemySpawnInfo.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpawnManager.generated.h"

class ASpawnPoint;

/**
 * @brief The states for the spawner manager
 */
UENUM(BlueprintType)
enum class ESpawnManagerState : uint8
{
  ONGOING UMETA(DisplayName = "Ongoing"),
  WAITING UMETA(DisplayName = "Waiting"),
  FINISHED UMETA(DisplayName = "Finished")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnManagerStateChanged, ESpawnManagerState, currentState);

/**
 * @brief This class defines the spawner manager for the spawn points
 */
UCLASS()
class PROJECTSINGULARITY_API USpawnManager : public UWorldSubsystem
{
  GENERATED_BODY()

#pragma region | Variables

public:
  /**
   * @brief Delegate that broadcasts the current state of the manager
   */
  UPROPERTY(BlueprintAssignable)
  FOnSpawnManagerStateChanged OnStateChange;

  /**
   * @brief The wait time inbetween rounds
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13", meta = (DisplayName = "Time between rounds"))
  float timeBetweenRounds;


  /**
   * @brief Defines if this manager is progressive or not
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13", meta = (DisplayName = "Is Manager Progressive?"))
  bool isProgressive = false;

  /**
   * @brief Defines the multiplier to use to up the difficulty
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13", meta = (DisplayName = "Difficulty Multiplier"))
  float difficultyMultiplier = 1.2f;

  /**
   * @brief The extra enemies to add each round
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13", meta = (DisplayName = "Extra Enemies Per Round"))
  int32 extraEnemiesPerRound = 5;

private:
  /**
   * @brief Stores the current round to start
   */
  int32 m_currentRound;

  /**
   * @brief This holds the info of all the spawners that waiting for the next round
   */
  int32 m_spawnersOnHold;

  /**
   * @brief This holds the counter of all the spawners that are ready to start
   */
  int32 m_spawnersReady;

  /**
   * @brief Saves the initializaed spawners
   */
  TSet<TObjectPtr<ASpawnPoint>> m_initializedSpawners;

  /**
   * @brief The spawnpoint to manage
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13", meta = (DisplayName = "Spawners"))
  TArray<TObjectPtr<ASpawnPoint>> spawners;

  /**
   * @brief Handles the timer set inbetween rounds
   */
  UPROPERTY()
  FTimerHandle m_roundTimerHandle;

  /**
   * @brief Saves the current state of the manager
   */
  ESpawnManagerState m_currentState;

#pragma endregion

public:

  /**
   * @brief Called to initialize the subsystem
   * @param _rCollection 
   */
  virtual void Initialize(FSubsystemCollectionBase& _rCollection) override;

  /**
   * @brief Called to deinitialize the subsystem
   */
  virtual void Deinitialize() override;

  /**
   * @brief Will register the spawnpoint into the manager
   * @param _inSpawner The spawnpoint to save
   */
  void RegisterSpawnPoint(TObjectPtr<ASpawnPoint> _inSpawner);

  /**
   * @brief Will activate the manager and start the first round
   * @param _delay The delay before starting, default is 0.01
   */
  UFUNCTION(BlueprintCallable, Category = "Spawner")
  void Activate(float _delay = 0.01f);

protected:

  /**
   * @brief Used to start the next round and setup the saved spawners
   */
  void NextRound();

  /**
   * @brief For when the round ends
   */
  void OnRoundEnd();

  /**
   * @brief For when the round starts
   */
  void OnRoundStart();

private:
  /**
   * @brief Handles the state changes of the spawn points
   * @param _state the recived state
   */
  UFUNCTION()
  void SpawnPointStateChanged(ESpawnPointState _state, ASpawnPoint* _spawner);

  /**
   * @brief Sets up the receiving spawner
   * @param _spawner Reference to the spawner to setup
   */
  void SetupSpawner(TObjectPtr<ASpawnPoint> _spawner) const;

  /**
   * @brief Will set up the next round
   */
  void SetupRound();

  /**
   * @brief Will start the next round with delay
   */
  void StartNextRoundDelayed();
};
