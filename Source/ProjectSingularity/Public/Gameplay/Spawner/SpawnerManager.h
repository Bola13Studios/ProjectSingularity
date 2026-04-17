/************************************************************************
 * @description: This class defines the spawner manager for the spawn points
 * @author: Josephine Esposito
 * @date: 17/04/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnInfo.h"
#include "SpawnPoint.h"
#include "SpawnerManager.generated.h"

class ASpawnPoint;

UENUM(BlueprintType)
enum class ESpawnManagerState : uint8
{
  ONGOING UMETA(DisplayName = "Ongoing"),
  WAITING UMETA(DisplayName = "Waiting")
};

USTRUCT(BlueprintType)
struct FRoundSpawn
{
  GENERATED_BODY()

public:
  /**
   * @brief The list of enemies to spawn
   */
  UPROPERTY(EditAnywhere)
  TArray<FEnemySpawnInfo> info;

  UPROPERTY(EditAnywhere)
  int32 totalEnemies;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnManagerStateChanged, ESpawnManagerState, currentState);

/**
 * @brief This class defines the spawner manager for the spawn points
 */
UCLASS()
class PROJECTSINGULARITY_API ASpawnerManager : public AActor
{
  GENERATED_BODY()

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
   * @brief The list of rounds to spawn
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13", meta = (DisplayName = "Rounds"))
  TArray<FRoundSpawn> rounds;

  /**
   * @brief The spawnpoint to manage
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13", meta = (DisplayName = "Spawners"))
  TArray<TObjectPtr<ASpawnPoint>> spawners;

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

  TSet<ASpawnPoint*> m_initializedSpawners;

  UPROPERTY()
  FTimerHandle m_roundTimerHandle;

public:
  /**
   * @brief Sets default values for this actor's properties
   */
  ASpawnerManager();

protected:
  /**
   * @brief Called when the game starts or when spawned
   */
  virtual void BeginPlay() override;

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
  void SpawnPointStateChanged(ESpawnPointState _state, ASpawnPoint* _spawner);

  /**
   * @brief Sets up the receiving spawner
   * @param _spawner Reference to the spawner to setup
   */
  void SetupSpawner(TObjectPtr<ASpawnPoint> _spawner);

  void SetupRound();

  void StartNextRoundDelayed();
};
