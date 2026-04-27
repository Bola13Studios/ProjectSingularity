/************************************************************************
 * @description: This class defines an enemy spawner actor that is responsible
 *               for spawning enemy characters in the game.
 * @author: Lucia Fernandez & Josephine Esposito
 * @date: 17/03/2026
 * @edited_by: Josephine - 24/04/2026
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnInfo.h"
#include "SpawnPoint.generated.h"

class ABaseEnemy;
class ASpawnPoint;

/**
 * @brief Defines the states of the spawner
 */
UENUM(BlueprintType)
enum class ESpawnPointState : uint8
{
  IDLE UMETA(DisplayName = "Idle"),
  ISREADY UMETA(DisplayName = "Is Ready"),
  SPAWNING UMETA(DisplayName = "Spawning"),
  WAITING UMETA(DisplayName = "Waiting For Enemies To Die"),
  FINISHED UMETA(DisplayName = "Finished all the round")
};

/**
 * @brief The structure for the rounds
 */
USTRUCT(BlueprintType)
struct FRoundSpawn
{
  GENERATED_BODY()

public:
  /**
   * @brief The list of enemies to spawn
   */
  UPROPERTY(EditAnywhere)
  TSubclassOf<ABaseEnemy> enemyClass;

  /**
   * @brief The total number of enemies to spawn per round
   */
  UPROPERTY(EditAnywhere)
  int32 totalEnemies;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpawnPointStateChanged, ESpawnPointState, State, ASpawnPoint*,
                                             SpawnPoint);

/**
 * @brief This class defines an enemy spawner actor that is responsible
 *        for spawning enemy characters in the game.
 */
UCLASS()
class PROJECTSINGULARITY_API ASpawnPoint : public AActor
{
  GENERATED_BODY()

public:
  /**
   * @brief The variation that will be applied to the spawn location when spawning an actor. This allows for some
   * randomness in the spawn location, making it less predictable and more dynamic.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13", meta = (DisplayName = "Spawn Variation"))
  FVector2D spawnVariation;

  /**
   * @brief The list of rounds to spawn
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13", meta = (DisplayName = "Rounds"))
  TArray<FRoundSpawn> rounds;

  /**
   * @brief Delegates that is broadcasted when the spawn point changes its state. This allows other classes to react to
   * the state change
   */
  UPROPERTY(BlueprintAssignable, Category = "Bola 13")
  FOnSpawnPointStateChanged OnSpawnPointStateChanged;

  /**
   * @brief Defines the intervall between each spawn
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13", meta = (DisplayName = "Spawn Interval"))
  float spawnInterval = 0.2f;

  /**
   * @brief Defines a how many enemies to spawn per tick
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13", meta = (DisplayName = "Spawn per tick"))
  int32 spawnPerTick = 2;

private:
  /**
   * @brief Saves the number of entities currently spawned by this spawn point. This is used to keep track of how many
   * entities are spawned
   */
  int32 m_currentSpawnedEntities;

  /**
   * @brief The total number of enemies to spawn
   */
  int32 m_totalEntitiesToSpawn;

  /**
   * @brief The class of enemies to spawn
   */
  TSubclassOf<ABaseEnemy> m_enemyType;

  /**
   * @brief Handles the timer used to spawn enemies
   */
  UPROPERTY()
  FTimerHandle m_spawnTimerHandle;

  /**
   * @brief Counts how many enemies we have spawned so far
   */
  int32 m_spawnedSoFar = 0;

public:
  /**
   * @brief Sets default values for this actor's properties
   */
  ASpawnPoint();

  /**
   * @brief This will start the spawning process
   */
  void StartSpawning();

  /**
   * @brief This will setup the spawn point
   * @param _roundIndex The index of the round to spawn
   */
  void Setup(int32 _roundIndex);

protected:
  /**
   * @brief Called when the game starts or when spawned
   */
  virtual void BeginPlay() override;

private:
  /**
   * @brief This will update the array and counter everytime an enemy dies
   * @param Enemy
   */
  UFUNCTION()
  void EnemyDied();

  /**
   * @brief Will spawn enemies after predetermined tick and not all at once
   */
  void SpawnTick();
};
