/************************************************************************
 * @description: Represents the structure for the Hype Multiplier table
 * @author: Josephine Esposito
 * @date: 06/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "HypeMultipliers.generated.h"

/**
 * Represents the structure for the Hype Multiplier table
 */
USTRUCT(BlueprintType)
struct FHypeMultipliers : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float CriticalMultiplier = 1.5f;

	UPROPERTY(EditAnywhere)
	float StreakMultiplier = 1.2f;

	UPROPERTY(EditAnywhere)
	float MultiKillMultiplier = 1.3f;

	UPROPERTY(EditAnywhere)
	float RiskActionMultiplier = 2.0f;
};
