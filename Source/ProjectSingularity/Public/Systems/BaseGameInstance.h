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
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Project", meta = (DisplayName = "Version"))
  FName m_version;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Project", meta = (DisplayName = "Can Save Events"))
  bool canSaveEvents = true;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Project", meta = (DisplayName = "Can Save Stats"))
  bool canSaveStats = true;

  /**
   * @brief The wait time inbetween rounds
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project|Spawners",
            meta = (DisplayName = "Time between rounds"))
  float timeBetweenRounds = 10.0f;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Asset|Chaser Enemy",
            meta = (DisplayName = "Chaser Data Asset"))
  TObjectPtr<UChaserEnemyDataAsset> m_pChaserDataAsset;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Asset|Enemy Config",
            meta = (DisplayName = "General Enemy Config Data Asset"))
  TObjectPtr<UEnemyConfigDataAsset> m_EnemyConfigDataAsset;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Asset|Weapon",
            meta = (DisplayName = "Weapon Data Asset"))
  TObjectPtr<UWeaponsDataAsset> m_WeaponDataAsset;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Asset|Player",
            meta = (DisplayName = "Player Config Data Asset"))
  TObjectPtr<UPlayerConfigDataAsset> m_PlayerDataAsset;

  /**
   * Holds the needed points for each popularity level and its related multiplier
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table|Hype",
            meta = (DisplayName = "Hype Popularity Data Table"))
  TObjectPtr<UDataTable> m_hypePopularityDataTable;

  /**
   * Hold the needed values for the appliable modifiers and its durations if needed
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table|Hype",
            meta = (DisplayName = "Hype Modifiers Data Table"))
  TObjectPtr<UDataTable> m_hypeModifiersDataTable;
};
