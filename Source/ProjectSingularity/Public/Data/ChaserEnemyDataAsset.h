/************************************************************************
 * @description: This class defines a chaser enemy data asset that inherits from the UEnemyConfigDataAsset class. The
 *chaser enemy data asset
 * @author: Lucia Fernandez
 * @date: 25/02/2026
 * @edited_by: <co editor>
 *             <co editor>
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "EnemyConfigDataAsset.h"
#include "ChaserEnemyDataAsset.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class PROJECTSINGULARITY_API UChaserEnemyDataAsset : public UEnemyConfigDataAsset
{
	GENERATED_BODY()
public:

	// Properties specific to the chaser enemy character
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Stagger Timer"))
	float m_timeStager;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Pause Time"))
	float m_timePause;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Attack Range"))
	float m_attackRange;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Dash Force"))
	float m_dashForce;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Attack Collider Position"))
  FVector m_attackCollider;

};
