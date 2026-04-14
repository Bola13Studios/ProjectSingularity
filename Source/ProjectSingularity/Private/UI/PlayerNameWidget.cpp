/************************************************************************
 * @description: Widget shown at the start of a session for the player
 *               to enter their display name.
 * @author: Serra
 * @date: 14/04/2026
 * @edited_by:
 ************************************************************************/

#include "ProjectSingularity/Public/UI/PlayerNameWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectSingularity/Public/Systems/BaseGameInstance.h"
#include "ProjectSingularity/Public/Systems/PlayerSaveGame.h"

#pragma region | PROTECTED METHODS

void UPlayerNameWidget::NativeConstruct()
{
  Super::NativeConstruct();

  if (m_pNameInputBox)
  {
    m_pNameInputBox->OnTextChanged.AddDynamic(this, &UPlayerNameWidget::OnNameTextChanged);
    m_pNameInputBox->OnTextCommitted.AddDynamic(this, &UPlayerNameWidget::OnNameTextCommitted);
    m_pNameInputBox->SetFocus();
  }

  if (m_pConfirmButton)
  {
    m_pConfirmButton->OnClicked.AddDynamic(this, &UPlayerNameWidget::OnConfirmClicked);
  }
}

#pragma endregion

#pragma region | PRIVATE METHODS

void UPlayerNameWidget::OnNameTextChanged(const FText& text)
{
  const FString currentText = text.ToString();

  if (currentText.Len() > MaxPlayerNameLength)
  {
    m_pNameInputBox->SetText(FText::FromString(currentText.Left(MaxPlayerNameLength)));
  }
}

void UPlayerNameWidget::OnNameTextCommitted(const FText& text, ETextCommit::Type commitMethod)
{
  if (commitMethod == ETextCommit::OnEnter)
  {
    OnConfirmClicked();
  }
}

void UPlayerNameWidget::OnConfirmClicked()
{
  if (!m_pNameInputBox)
  {
    return;
  }

  const FString playerName = m_pNameInputBox->GetText().ToString().TrimStartAndEnd();

  if (playerName.IsEmpty() || playerName.Len() > MaxPlayerNameLength)
  {
    return;
  }

  // Save the name to the session data.
  if (UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    gameInstance->m_sessionData.m_sPlayerName = playerName;
  }

  // Persist the name to disk so it can be reloaded next session.
  UPlayerSaveGame* saveGame = Cast<UPlayerSaveGame>(
    UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass())
  );
  if (saveGame)
  {
    saveGame->m_sPlayerName = playerName;
    UGameplayStatics::SaveGameToSlot(saveGame, TEXT("PlayerData"), 0);
  }

  // Notify listeners (e.g. GameHUDSetUp) that the name has been confirmed.
  OnPlayerNameConfirmed.Broadcast(playerName);

  RemoveFromParent();
}

#pragma endregion
