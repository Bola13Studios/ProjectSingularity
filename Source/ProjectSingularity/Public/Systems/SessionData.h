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
};
