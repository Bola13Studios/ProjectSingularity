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
