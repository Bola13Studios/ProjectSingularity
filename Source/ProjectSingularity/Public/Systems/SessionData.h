/************************************************************************
 * @description: Holds the base structure of the statistics to save
 * @author: Josephine Esposito
 * @date: 24/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "SessionData.generated.h"

/**
 * Holds the base structure of the statistics to save
 */
USTRUCT()
struct FSessionData
{
  GENERATED_BODY()

  /**
   * @brief This holds all the stats to save. Careful with the typos
   */
  UPROPERTY()
  TMap<FName, float> stats;

  /**
   * @brief Retrieves the value of a named statistic.
   * @param _statName The name/identifier of the statistic to look up.
   * @return The statistic's value as a float if found; otherwise returns 0.0f.
   */
  float GetStat(FName _statName) const
  {
    if (const float* statValue_ = stats.Find(_statName))
    {
      return *statValue_;
    }
    return 0.f; // default value if the stat is not found
  }
};
