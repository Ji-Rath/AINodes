using UnrealBuildTool;
 
public class SoldierAI : ModuleRules
{
	public SoldierAI(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "AIModule", "GameplayTasks"});


		PublicIncludePaths.AddRange(new string[] {"SoldierAI/Public"});
		PrivateIncludePaths.AddRange(new string[] {"SoldierAI/Private"});
	}
}