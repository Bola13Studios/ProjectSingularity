/************************************************************************
* @description: Widget used to display player hype points in the HUD
 * @author: Serra
 * @date: 10/04/2026
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HypeWidget.generated.h"

#pragma region | FORWARD DECLARATIONS

class UTextBlock;
class UHypeComponent;

#pragma endregion

UCLASS()
class PROJECTSINGULARITY_API UHypeWidget : public UUserWidget
{
  GENERATED_BODY()

#pragma region | VARIABLES

protected:

  /**
   * @brief Text block used to display the current hype value
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_hypeText;

private:

  /**
   * @brief Cached hype component reference
   */
  UPROPERTY()
  TObjectPtr<UHypeComponent> m_hypeComp;

#pragma endregion

#pragma region | METHODS

public:

  /**
   * @brief Bind this widget to a hype component
   * @param _inHypeComp hype component to bind
   */
  UFUNCTION(BlueprintCallable)
  void BindToHypeComponent(UHypeComponent* _inHypeComp);

protected:

  /**
   * @brief Called when the widget is being destroyed
   */
  virtual void NativeDestruct() override;

private:

  /**
   * @brief Callback executed when hype changes
   * @param _currentHype current hype value
   * @param _delta hype variation
   */
  UFUNCTION()
  void HandleHypeChanged(int _currentHype, int _delta);

  /**
   * @brief Refresh the hype UI value
   * @param _currentHype current hype value
   */
  void RefreshUI(int _currentHype);

#pragma endregion
};