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

	/**
	 * The level of the hype, starting from 0. Each level corresponds to a range of hype points defined by MinRequiredHype and MaxRequiredHype.
	 */
	UPROPERTY(EditAnywhere)
	int32 level = 0;

	/*
	 * The minimum amount of hype required to reach this level.
	 */
	UPROPERTY(EditAnywhere)
	int32 minRequiredHype = 0;

	/**
	 * The maximum amount of hype required to reach this level.
	 */
	UPROPERTY(EditAnywhere)
	int32 maxRequiredHype = 0;
};
//EOF