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

private:
  /**
   * @brief An integer property that stores the current kill strike count
   */
  UPROPERTY()
  int m_killStrikeCount = 0;

  /**
   * @brief Time window in seconds to count kill strike
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Hype", meta = (DisplayName = "Kill Strike Rate"))
  float m_killStrikeWindow = 2.0f;

  /**
   * @brief The timer handle used to track the kill strike window
   */
  FTimerHandle m_killStrikeTimer;

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
   */
  UFUNCTION(BlueprintCallable)
  void RegisterKill(UHypeSourceComponent* _source, const bool& critical);

  /**
   * @brief Used to register a multi-kill
   * @param _killCount 
   */
  void RegisterMultiKill(int32 _killCount);

  /**
   * @brief Registers a strike-kill event.
   */
  void RegisterStrikeKill();

  /**
   * @brief Resets the strike-kill state, clearing any current strike-kill count or progress.
   */
  void ResetStrikeKill();

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

  void AddExternalModifier(const FName _modifier);

protected:
  // Called when the game starts
  void BeginPlay() override;
};
