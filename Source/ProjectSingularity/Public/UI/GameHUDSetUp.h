/************************************************************************
* @description: HUD setup class responsible for creating and binding HUD widgets
 * @author: Serra
 * @date: 10/04/2026
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "HUDWidget.h"
#include "Gameplay/Character/Player/PlayerCharacter.h"
#include "GameFramework/HUD.h"
#include "GameHUDSetUp.generated.h"


UCLASS()
class PROJECTSINGULARITY_API AGameHUDSetUp : public AHUD
{
  GENERATED_BODY()

#pragma region | VARIABLES

public:
  /**
   * @brief Cached player controller reference
   */
  UPROPERTY()
  APlayerController* m_playerController = nullptr;

  /**
   * @brief Runtime list containing active game menu widgets
   */
  UPROPERTY(BlueprintReadWrite)
  TArray<UUserWidget*> m_gameMenusWidgetList{};

  /**
   * @brief HUD widget class configured in Blueprint
   */
  UPROPERTY(EditDefaultsOnly, Category = "UI")
  TSubclassOf<UHUDWidget> m_hudWidgetClass{};

  /**
   * @brief Runtime HUD widget instance
   */
  UPROPERTY(BlueprintReadWrite)
  UHUDWidget* m_hudWidget = nullptr;

private:
  /**
   * @brief Cached game instance reference
   */
  UPROPERTY()
  UGameInstance* m_gameInstance = nullptr;

#pragma endregion

#pragma region | METHODS

protected:
  /**
   * @brief Called when the game starts
   */
  virtual void BeginPlay() override;

private:
  /**
   * @brief Initialize all game menus used during gameplay
   */
  UFUNCTION()
  void InitializeAllGameMenus();

  /**
   * @brief Attempt to bind the HUD to the current possessed pawn
   */
  UFUNCTION()
  void TryBindHUDToPawn() const;

#pragma endregion
};
