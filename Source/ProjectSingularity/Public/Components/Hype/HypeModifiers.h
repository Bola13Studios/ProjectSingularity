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
  UPROPERTY(EditAnywhere, meta = (DisplayName = "Identifier"))
  FName id = "Default";

  /**
   * The value of the modifier based on percentages. From 0 to 1 max
   */
  UPROPERTY(EditAnywhere, meta = (DisplayName = "Multiplier Value", ClampMin = 0, ClampMax = 1))
  float value = 0.5f;

  /**
   * The duration of the modifier. If the flag is false, this value will be ignored
   */
  UPROPERTY(EditAnywhere, meta = (DisplayName = "Duration", ClampMin = 0.0f, Tooltip = "This value will be ignored if isTemporary is set to FALSE"))
  float duration = 0;

  /**
   * The flag stating if the modifier is time based or not
   */
  UPROPERTY(EditAnywhere, meta = (DisplayName = "Is it Temporary?", Tooltip = "This value has the priority above the duration"))
  bool isTemporary = false;
};
//EOF