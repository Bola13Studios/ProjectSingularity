/************************************************************************
 * @description: Defines the possible high-level states of the game.
 * @author: Jaime Paramo
 * @date: 17/03/2026
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameStateEnum.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
  PLAYING  UMETA(DisplayName = "Playing"),
  PAUSED   UMETA(DisplayName = "Paused"),
  GAMEOVER UMETA(DisplayName = "Game Over")
};
