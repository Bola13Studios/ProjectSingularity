/************************************************************************
 * @description: Handles the popularity for the hype
 * @author: Josephine Esposito
 * @date: 31/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PopularityComponent.generated.h"

struct FHypeLevels;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSINGULARITY_API UPopularityComponent : public UActorComponent
{
  GENERATED_BODY()

#pragma region | Variables

public:
  /**
   * @brief A raw pointer to an FHypeLevels instance, typically used to hold or reference hype level data.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  float multiplier = 0.0f;

  /**
   * @brief An integer variable representing the current popularity level
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  int32 level = 0;

private:

  /**
   * @brief Stores the current popularity as a floating-point value.
   */
  float m_currentPopularity;

  /**
   * @brief Maximum popularity value.
   */
  float m_maxPopularity = 10000.f;

  /**
   * @brief A handle that identifies and manages a decay timer.
   */
  FTimerHandle m_decayTimerHandle;

  /**
   * @brief A floating-point interval value used to represent a decay duration or rate.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Popularity")
  float m_decayInterval = 0.2f;

  /**
   * @brief A floating-point variable representing the decay rate used in computations.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Popularity")
  float m_decayRate = 5.0f;

#pragma endregion

public:
  // Sets default values for this component's properties
  UPopularityComponent();

  /**
   * @brief Will get the current popularity score
   * @note This value could change based on time
   */
  float GetCurrentPopularity() const;

  /**
   * @brief Will return the current popularity level
   */
  int GetPopularityLevel() const;

  /**
   * Used to increase the popularity value
   */
  void IncreasePopularity(float _value);

  /**
   * @brief Used to decrease the popularity value
   */
  void DecreasePopularity(float _value);

  /**
   * @brief Used to update the popularity level
   */
  void UpdateLevel();

  /**
   * @brief Will return the multiplier based on the current popularity level
   */
  float GetMultiplier() const;

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

  // Called when the game ends
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
  /**
   * @brief Performs decay handling operations.
   */
  void HandleDecay();

  /**
   * @brief Calculates the decayed popularity value based on the provided current popularity. This is a const member
   * function and does not modify the object.
   * @return The popularity value after applying decay (float).
   */
  float CalculateDecay() const;
};
// EOF
