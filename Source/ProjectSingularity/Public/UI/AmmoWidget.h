/************************************************************************
* @description: Widget used to display ammo information in the HUD
 * @author: Serra
 * @date: 10/04/2026
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

UCLASS()
class PROJECTSINGULARITY_API UAmmoWidget : public UUserWidget
{
  GENERATED_BODY()

#pragma region | METHODS

public:
  /**
   * @brief Default widget constructor
   */
  UAmmoWidget() = default;

#pragma endregion
};
