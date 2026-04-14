/************************************************************************
 * @description: HUD widget that displays a welcome message with the
 *               player's name and a random player number.
 * @author: Serra
 * @date: 14/04/2026
 * @edited_by:
 ************************************************************************/

#include "ProjectSingularity/Public/UI/WelcomeWidget.h"
#include "Components/TextBlock.h"
#include "Math/UnrealMathUtility.h"
#include "ProjectSingularity/Public/Systems/BaseGameInstance.h"

#pragma region | PROTECTED METHODS

void UWelcomeWidget::NativeConstruct()
{
  Super::NativeConstruct();

  m_iPlayerNumber = FMath::RandRange(0, 999);

  RefreshWelcomeText();
}

#pragma endregion

#pragma region | PRIVATE METHODS

void UWelcomeWidget::RefreshWelcomeText()
{
  if (!m_pWelcomeText)
  {
    return;
  }

  FString playerName;

  if (const UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    playerName = gameInstance->m_sessionData.m_sPlayerName;
  }

  const FString welcomeString = FString::Printf(
    TEXT("Jugador %d: %s"),
    m_iPlayerNumber,
    *playerName
  );

  m_pWelcomeText->SetText(FText::FromString(welcomeString));
}

#pragma endregion
