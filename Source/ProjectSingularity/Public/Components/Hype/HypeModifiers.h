/************************************************************************
 * @description: Represents the structure for the Hype Modifiers Table
 * @author: Josephine Esposito
 * @date: 31/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "HypeLevels.generated.h"

/**
 * Represents the structure for the Hype Modifiers Table
 */
USTRUCT(BlueprintType)
struct FHypeModifiers : public FTableRowBase
{
	GENERATED_BODY()

  /**
   * The name identifier of the modifier
   */
	UPROPERTY(EditAnywhere)
  FName id = "Default";

  /**
   * The value of the modifier based on percentages. From 0 to 1 max
   */
	UPROPERTY(EditAnywhere)
  float value = 0.5f;

  /**
   * The duration of the modifier. If the flag is false, this value will be ignored
   */
	UPROPERTY(EditAnywhere)
  float duration = 0;

  /**
   * The flag stating if the modifier is time based or not
   */
	UPROPERTY(EditAnywhere)
  bool isTemporary = false;
};
//EOF