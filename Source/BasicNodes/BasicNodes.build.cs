using UnrealBuildTool;
 
public class BasicNodes : ModuleRules
{
	public BasicNodes(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "AIModule", "GameplayTasks"});
 
		PublicIncludePaths.AddRange(new string[] {"BasicNodes/Public"});
		PrivateIncludePaths.AddRange(new string[] {"BasicNodes/Private"});
	}
}