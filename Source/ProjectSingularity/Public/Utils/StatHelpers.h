/************************************************************************
 * @description: Holds the macros used to save stats
 * @author: Josephine Esposito
 * @date: 14/04/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "ProjectSingularity/Public/Systems/SessionData.h"

using FStatAccessor = TFunction<int32&(FSessionData&)>;

// Macro to easily access the stats in the session data struct
#define STAT_PATH(Path)           \
  [](FSessionData& Data) -> auto& \
  {                               \
    return Data.Path;             \
  }

#define STAT_MAP(Path)                                   \
  [](FSessionData& Data) -> TMap<EStationStates, int32>& \
  {                                                      \
    return Data.Path;                                    \
  }
