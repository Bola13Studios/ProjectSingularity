/************************************************************************
 * @description: Base class for Checkpoints
 * @author: Rubén Santos
 * @date: 07/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS()
class PROJECTSINGULARITY_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ACheckpoint();

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* triggerBox;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	static void RespawnPlayer();

	UPROPERTY(EditDefaultsOnly, Category = "Checkpoint");
	bool bShowGizmo;

private:
	void RespawnPlayerToActiveCheckpoint();
	void DrawGizmos();

	static ACheckpoint* m_activeCheckpoint;
};
