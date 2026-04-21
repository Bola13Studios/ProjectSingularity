// Fill out your copyright notice in the Description page of Project Settings.

/************************************************************************
 * @description: This class serves as the base enemy character for all enemy characters in the game. It inherits from
 *the ABaseCharacter class and provides additional functionality and properties specific to enemy characters. The class
 *includes a damage property to represent the amount of damage the enemy can inflict on the player, and a reference to
 *an enemy configuration data asset for storing enemy-specific data.
 * @author: Lucia Fernandez
 * @date: 25/02/2026
 * @edited_by: 
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/BaseCharacter.h"
#include "BaseEnemy.generated.h"



class UEnemyConfigDataAsset;
/**
 * 
 */
UCLASS()
class PROJECTSINGULARITY_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:

	 /**
   * @brief Constructor for the ABaseEnemy class. This constructor sets default values for the enemy character's
   * properties and initializes any necessary components. It is called when an instance of the ABaseEnemy class is
   * created.
   */
	ABaseEnemy();



protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Enemy damage property to represent the amount of damage the enemy can inflict on the player
	UPROPERTY(VisibleAnywhere, Category = "Stats|Enemy", meta = (DisplayName = "Enemy Damage"))
	float m_damage;

private:
  // Reference to an enemy configuration data asset for storing enemy-specific data
  TObjectPtr<UEnemyConfigDataAsset> m_EnemyDataAsset;
};
