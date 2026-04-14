// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectSingularity.h"

#if WITH_EDITOR
#include "ToolMenus.h"
#include "Kismet/GameplayStatics.h"
#endif

IMPLEMENT_PRIMARY_GAME_MODULE(FProjectSingularityModule, ProjectSingularity, "ProjectSingularity");

void FProjectSingularityModule::StartupModule()
{
#if WITH_EDITOR
  UToolMenus::RegisterStartupCallback(
    FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FProjectSingularityModule::RegisterMenuExtensions)
  );
#endif
}

#if WITH_EDITOR

void FProjectSingularityModule::RegisterMenuExtensions()
{
  UToolMenu* toolsMenu = UToolMenus::Get()->ExtendMenu("MainFrame.MainMenu.Tools");

  FToolMenuSection& section = toolsMenu->AddSection(
    "ProjectSingularityDevTools",
    NSLOCTEXT("ProjectSingularity", "DevToolsSection", "Dev Tools")
  );

  section.AddMenuEntry(
    "ClearAllSaveData",
    NSLOCTEXT("ProjectSingularity", "ClearSaveData", "Clear All Save Data"),
    NSLOCTEXT("ProjectSingularity", "ClearSaveDataTooltip", "Deletes all ProjectSingularity save game slots."),
    FSlateIcon(),
    FUIAction(FExecuteAction::CreateStatic(&FProjectSingularityModule::ClearAllSaveData))
  );
}

void FProjectSingularityModule::ClearAllSaveData()
{
  // Delete all save slots. Add new ones here as the game grows.
  UGameplayStatics::DeleteGameInSlot(TEXT("PlayerData"), 0);

  UE_LOG(LogTemp, Warning, TEXT("[SaveManager] All save data cleared."));
}

#endif
