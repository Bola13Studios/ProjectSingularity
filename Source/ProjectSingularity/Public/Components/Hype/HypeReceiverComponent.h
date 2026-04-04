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
class UDataTable; // struct FHypeMultipliers;
class UHypeSourceComponent;
class UHypeCalculatorComponent;
class UHypeModifierComponent;
class UPopularityComponent;
#pragma endregion

/**
 * @brief Represents the Hype Component for the Receivers such as the player
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSINGULARITY_API UHypeReceiverComponent : public UHypeComponent
{
  GENERATED_BODY()

#pragma region | Attributes

public:
  /**
   * @brief Holds the current hype level for this receiver
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Hype", meta = (DisplayName = "Hype Level"))
  int m_currentHypeLevel;

  /**
   * @brief Hold the current kill streak for this receiver
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Hype", meta = (DisplayName = "Kill Streak"))
  int m_currentKillStreak;

private:
  /**
   * @brief An integer property that stores the current multi-kill count
   */
  UPROPERTY()
  int m_multiKillCount = 0;

  /**
   * @brief Time window in seconds to count multi-kills
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Hype", meta = (DisplayName = "MultiKill Rate"))
  float m_multiKillWindow = 2.0f;

  FTimerHandle m_multiKillTimer;

  /**
   * @brief Holds the reference to the data table with all the related multipliers
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Hype", meta = (DisplayName = "Multiplier Table"))
  UDataTable* m_hypeMultiplierTable;

  /**
   * @brief Holds the reference to the data table with all the related
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Hype", meta = (DisplayName = "Level Table"))
  UDataTable* m_hypeLevelTable;

  UPROPERTY()
  UHypeCalculatorComponent* m_calculatorComponent;

  UPROPERTY()
  UHypeModifierComponent* m_modifierComponent;

  UPROPERTY()
  UPopularityComponent* m_popularityComponent;
#pragma endregion

public:
  /**
   * @brief Used to register a kill
   * @param _Source the killed source
   * @param Critical if it was a critical hit (weakpoint)
   * @param MultiKill if it was a multi kill or not
   */
  UFUNCTION(BlueprintCallable)
  void RegisterKill(UHypeSourceComponent* _source, const bool& critical);

  /**
   * @brief Registers a multi-kill event.
   */
  void RegisterMultiKill();

  /**
   * @brief Resets the multi-kill state, clearing any current multi-kill count or progress.
   */
  void ResetMultiKill();

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
  bool IsHypeEnough(float _amount);

protected:
  // Called when the game starts
  void BeginPlay() override;
};
