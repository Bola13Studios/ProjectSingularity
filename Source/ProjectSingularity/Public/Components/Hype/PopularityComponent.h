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

private:
  TObjectPtr<FHypeLevels> m_hypeLevel;

  float m_currentPopularity;

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

public:
  // Called every frame
  virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                             FActorComponentTickFunction* ThisTickFunction) override;
};
// EOF
