using UnrealBuildTool;
 
public class SoldierAI : ModuleRules
{
	public SoldierAI(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "AIModule", "GameplayTasks"});
		
	}
}