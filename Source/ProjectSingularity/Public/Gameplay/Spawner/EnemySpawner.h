/************************************************************************
 * @description: Behaviour of the enemy spawner
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
	// Sets default values for this actor's properties
	AEnemySpawner();
	void SpawnEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13|Components")
	TObjectPtr<UBoxComponent> m_spawnArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Spawning", meta = (DisplayName = "Enemy Type to Spawn"))
	TSubclassOf<ABaseEnemy> m_enemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (DisplayName = "Enemies to Spawn"))
	int m_enemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (DisplayName = "Spawn Delay"))
	float m_spawnDelay = 3.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:

	FTimerHandle SpawnTimerHandle;
};
