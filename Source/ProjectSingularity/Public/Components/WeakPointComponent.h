/************************************************************************
 * @description: Represents the Weak Point Component for the source,
 *               this will increase the damange when hit
 * @author: Josephine Esposito
 * @date: 05/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "WeakPointComponent.generated.h"

/**
 * @brief Represents the Weak Point Component for the source, this will increase the damange when hit
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSINGULARITY_API UWeakPointComponent : public USphereComponent
{
  GENERATED_BODY()

  public:
  /**
     * @brief Will return the damage multiplier of the weak point, this will be used to calculate the damage when hit
   * @return 
   */
    UFUNCTION(BlueprintCallable)
  float GetDamageMultiplier() const;
};
