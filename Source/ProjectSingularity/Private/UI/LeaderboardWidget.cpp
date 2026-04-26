/************************************************************************
 * @description: Widget that displays the full local leaderboard at the
 *               end of a session, listing all players ranked by score.
 * @author: Serra
 * @date: 26/04/2026
 * @edited_by:
 ************************************************************************/

#include "UI/LeaderboardWidget.h"
#include "UI/LeaderboardRowWidget.h"
#include "Components/ScrollBox.h"

void ULeaderboardWidget::PopulateLeaderboard(const TArray<FLeaderboardEntry>& entries)
{
  if (!m_pEntriesBox || !m_rowWidgetClass) return;

  m_pEntriesBox->ClearChildren();

  for (int32 i = 0; i < entries.Num(); ++i)
  {
    ULeaderboardRowWidget* row = CreateWidget<ULeaderboardRowWidget>(GetOwningPlayer(), m_rowWidgetClass);
    if (row)
    {
      row->InitRow(i + 1, entries[i].playerName, entries[i].score);
      m_pEntriesBox->AddChild(row);
    }
  }
}
