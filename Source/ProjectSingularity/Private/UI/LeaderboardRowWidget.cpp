/************************************************************************
 * @description: Widget that displays a single leaderboard entry row,
 *               showing rank, player name and score.
 * @author: Serra
 * @date: 26/04/2026
 * @edited_by:
 ************************************************************************/

#include "UI/LeaderboardRowWidget.h"
#include "Components/TextBlock.h"

void ULeaderboardRowWidget::InitRow(int32 rank, const FString& playerName, int32 score)
{
  if (m_pRankText)
    m_pRankText->SetText(FText::AsNumber(rank));

  if (m_pNameText)
    m_pNameText->SetText(FText::FromString(playerName));

  if (m_pScoreText)
    m_pScoreText->SetText(FText::AsNumber(score));
}
