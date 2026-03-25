/************************************************************************
 * @description: This class defines an enemy configuration data asset that inherits from the UCharacterConfigDataAsset
 *class. The enemy
 * @author: Lucia Fernandez
 * @date: 25/02/2026
 * @edited_by: <co editor>
 *             <co editor>
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/CharacterConfigDataAsset.h"
#include "EnemyConfigDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTSINGULARITY_API UEnemyConfigDataAsset : public UCharacterConfigDataAsset
{
	GENERATED_BODY()
	
public:

	// Enemy damage property to represent the amount of damage the enemy can inflict on the player
	UPROPERTY(EditAnywhere,BlueprintReadOnly, meta = (DisplayName = "Enemy Damage"))
	float damage;


};
