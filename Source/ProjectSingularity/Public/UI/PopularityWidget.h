/************************************************************************
 * @description: Represents the widget for displaying the player's
 * popularity level and related information in the UI
 * @author: Josephine Esposito
 * @date: 08/04/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopularityWidget.generated.h"

class UPopularityComponent;
class UTextBlock;
class UProgressBar;

/**
 * Represents the widget for displaying the player's
 * popularity level and related information in the UI
 */
UCLASS()
class PROJECTSINGULARITY_API UPopularityWidget : public UUserWidget
{
  GENERATED_BODY()

  protected:
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UProgressBar> m_popularityBar;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_multiplierText;

  private:
  UPROPERTY()
  TObjectPtr<UPopularityComponent> m_popularityComponent;

public:
  UFUNCTION(BlueprintCallable)
  void BindToPopularityComponent(UPopularityComponent* _inPopularityComp);

protected:
  virtual void NativeDestruct() override;

  private:
  void RefreshUI();
};
