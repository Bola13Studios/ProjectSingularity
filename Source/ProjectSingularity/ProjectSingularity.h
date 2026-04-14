// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FProjectSingularityModule : public FDefaultGameModuleImpl
{
public:
  virtual void StartupModule() override;

#if WITH_EDITOR
private:
  void RegisterMenuExtensions();
  static void ClearAllSaveData();
#endif
};
