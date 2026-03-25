/************************************************************************
 * @description: This class defines an enemy spawner actor that is responsible for spawning enemy characters in the
 *game. The spawner
 * @author: Lucia Fernandez
 * @date: 07/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class UBoxComponent;
class ABaseEnemy;

UCLASS()
class PROJECTSINGULARITY_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	  /**
   * @brief Constructor for the AEnemySpawner class. This constructor sets default values for the enemy spawner's
   * properties and initializes any necessary components. It is called when an instance of the AEnemySpawner class is
   * created.
   */
	AEnemySpawner();

	  /**
   * @brief Function to spawn an enemy character. This function is responsible for creating an instance of the specified
   * enemy type
   */
	void SpawnEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Box component to define the area where enemies will be spawned
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13|Components")
	TObjectPtr<UBoxComponent> m_spawnArea;

	// Enemy type to spawn, specified as a subclass of ABaseEnemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Bola13|Spawning", meta = (DisplayName = "Enemy Type to Spawn"))
	TSubclassOf<ABaseEnemy> m_enemyType;

	// Number of enemies to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola13|Spawning", meta = (DisplayName = "Enemies to Spawn"))
	int m_enemyCount;

	// Delay between each enemy spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola13|Spawning", meta = (DisplayName = "Spawn Delay"))
	float m_spawnDelay = 3.0f;

	// Whether the spawner should automatically respawn enemies after they are defeated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola13|Spawning", meta = (DisplayName = "Reload Spawner"))
	bool m_reloadSpawn;

	// Delay before the spawner can spawn a new wave of enemies after the previous wave has been defeated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola13|Spawning", meta = (DisplayName = "Spawn Wave Delay"))
	int m_waveCooldown;

	// Counter to keep track of the number of enemies spawned
	int m_enemiesSpawned = 0;

	  /**
   * @brief Function to reset the spawner. This function is responsible for resetting the spawner's state, allowing it
   * to
   */
	void ResetSpawner();
private:

	// Timer handle for managing the spawn delay between enemy spawns
	FTimerHandle SpawnTimerHandle;
};
