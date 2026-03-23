/************************************************************************
 * @description: Represents the Hype Component for the Receivers such as
                 the player
 * @author: Josephine Esposito
 * @date: 05/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "HypeComponent.h"
#include "HypeReceiverComponent.generated.h"

#pragma region | Forward Declaration
class UHypeSourceComponent;
class UDataTable; // struct FHypeMultipliers;
#pragma endregion

/**
 * @brief Represents the Hype Component for the Receivers such as the player
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSINGULARITY_API UHypeReceiverComponent : public UHypeComponent
{
  GENERATED_BODY()

public:
  /**
   * @brief Holds the current hype level for this receiver
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Hype", meta = (DisplayName = "Hype Level"))
  int m_CurrentHypeLevel;

  /**
   * @brief Hold the current kill streak for this receiver
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Hype", meta = (DisplayName = "Kill Streak"))
  int m_CurrentKillStreak;

private:
  /**
   * @brief Holds the reference to the data table with all the related multipliers
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Hype", meta = (DisplayName = "Multiplier Table"))
  UDataTable* m_HypeMultiplierTable;

  /**
   * @brief Holds the reference to the data table with all the related
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Hype", meta = (DisplayName = "Level Table"))
  UDataTable* m_HypeLevelTable;

public:
  /**
   * @brief Used to register a kill
   * @param _Source the killed source
   * @param Critical if it was a critical hit
   * @param MultiKill if it was a multi kill or not
   */
  UFUNCTION(BlueprintCallable)
  void RegisterKill(UHypeSourceComponent* _Source, const bool& Critical, const int& MultiKill);

  /**
   * @brief Used to Update the Current Hype Level
   */
  UFUNCTION(BlueprintCallable)
  void UpdateHypeLevel();

  /**
   * @brief Verifies if the current saved amount of hype is enough
   * @param _Amount The amount to check onto
   * @return A flag stating true if the amount is less or equal the current hype
   */
  UFUNCTION(BlueprintCallable)
  bool IsHypeEnough(float _Amount);
};
