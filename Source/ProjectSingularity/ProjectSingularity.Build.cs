using UnrealBuildTool;

public class ProjectSingularity : ModuleRules {
  public ProjectSingularity(ReadOnlyTargetRules Target) : base(Target) {
    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

    PublicDependencyModuleNames.AddRange(new string[] {
      "Core",
      "CoreUObject",
      "Engine",
      "InputCore",
      "EnhancedInput",
      "UMG",
      "NavigationSystem"
    });

    PrivateDependencyModuleNames.AddRange(new string[] {
      "Slate",
      "SlateCore"
    });
  }
}