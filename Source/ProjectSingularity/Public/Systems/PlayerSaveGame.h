/************************************************************************
 * @description: Save game class that persists the player's session data
 *               between play sessions.
 * @author: Serra
 * @date: 14/04/2026
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"

UCLASS()
class PROJECTSINGULARITY_API UPlayerSaveGame : public USaveGame
{
  GENERATED_BODY()

public:
  /**
   * @brief The player's display name saved from the previous session.
   */
  UPROPERTY()
  FString m_sPlayerName;
};
