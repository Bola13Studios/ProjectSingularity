/************************************************************************
 * @description: Represents the structure for the Hype Levels table
 * @author: Josephine Esposito
 * @date: 12/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "HypeLevels.generated.h"

/**
 * Represents the structure for the Hype Levels table
 */
USTRUCT(BlueprintType)
struct FHypeLevels : public FTableRowBase
{
  GENERATED_BODY()

public:
  /**
   * The level of the hype, starting from 0. Each level corresponds to a range of hype points defined by MinRequiredHype
   * and MaxRequiredHype.
   */
  UPROPERTY(EditAnywhere)
  int32 level = 0;

  /*
   * The required amount of hype to reach the level.
   */
  UPROPERTY(EditAnywhere)
  int32 requiredValue = 0;

  /**
   * The multiplier to apply to each level
   */
  UPROPERTY(EditAnywhere)
  float multiplier = 0.0f;

  /**
   * @brief The rate at which the popularity decays over time when the player is not actively generating hype
   */
  UPROPERTY(EditAnywhere, meta = (DisplayName = "Decay on the popularity level", ClampMin = 0))
  float decayRate;
};
// EOF
