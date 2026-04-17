/************************************************************************
 * @description: Represents the different states of the station
 * @author: Josephine Esposito
 * @date: 22/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"

/**
 * Represents the different states of the station
 */
UENUM(BlueprintType)
enum class EStationStates : uint8
{

  HEALTH      UMETA(DisplayName = "Health"),

  AMMO        UMETA(DisplayName = "Ammo"),

  MAX_HEALTH  UMETA(DisplayName = "Max Health"),

  DAMAGE      UMETA(DisplayName = "Damage"),

};