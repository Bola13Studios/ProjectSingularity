// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"


class UChaserEnemyDataAsset;
class UEnemyConfigDataAsset;
class UCharacterConfigDataAsset;
class UWeaponsDataAsset;
class UPlayerConfigDataAsset;
/**
 * 
 */
UCLASS()
class PROJECTSINGULARITY_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
		public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Asset|Chaser Enemy",meta = (DisplayName = "Chaser Data Asset"))
  TObjectPtr<UChaserEnemyDataAsset> m_pChaserDataAsset;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Asset|Weapon",meta = (DisplayName = "Weapon Data Asset"))
  TObjectPtr<UWeaponsDataAsset> m_WeaponDataAsset;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Asset|Player",meta = (DisplayName = "Player Config Data Asset"))
  TObjectPtr<UPlayerConfigDataAsset> m_PlayerDataAsset;
};
