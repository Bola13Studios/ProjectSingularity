/************************************************************************
 * @description: Represents the Hype Component for the source such as
                 the enemies or other objects
 * @author: Josephine Esposito
 * @date: 05/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "HypeComponent.h"
#include "HypeSourceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeakPointHit);

/**
 * @brief Represents the Hype Component for the source such as
 * the enemies or other objects
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSINGULARITY_API UHypeSourceComponent : public UHypeComponent
{
  GENERATED_BODY()

public:
  /**
   * @brief A Custom multiplier to apply to the base value based on percentage (from 0 to 1)
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Hype", meta = (DisplayName = "Custom Multiplier"))
  float m_baseMultiplier;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Hype", meta = (DisplayName = "Is it a Weakpoint?"))
  bool m_isWeakPoint;

  /**
   * @brief Temporary delegate used to broadcast when the actor attached to this component has been hit
   */
  UPROPERTY(BlueprintAssignable, Category = "Events")
  FOnWeakPointHit m_onHit;

private:
  /**
   * @brief Temporary flag that will tell if the component has already been activated
   * this is only for PoC
   */
  bool m_hasBeenHit;

public:
  /**
   * @brief Returns the base hype value
   * @return a rounded int
   */
  int GetHype() override;
};
