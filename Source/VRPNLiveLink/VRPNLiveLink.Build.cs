// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VRPNLiveLink : ModuleRules
{
	public VRPNLiveLink(ReadOnlyTargetRules Target) : base(Target)
	{
        bEnableExceptions = true;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
        PublicIncludePaths.AddRange
        (
            new string[]
            {
                "D:\\projects\\VRPN-Free-d.git\\submodules\\vrpn\\"
            }
        );


        PrivateIncludePaths.AddRange
        (
            new string[]
            {
                "D:\\projects\\VRPN-Free-d.git\\submodules\\vrpn\\"
            }
        );

        PublicDependencyModuleNames.AddRange
        (
            new string[]
            {
                "Core",
                "LiveLinkInterface",
                "Messaging",
            }
        );

        PrivateDependencyModuleNames.AddRange
        (
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore",
                "Json",
                "JsonUtilities",
                "Networking",
                "Sockets",
            }
        );

        PublicSystemLibraryPaths.AddRange
        (
            new string[]
            {
                "D:\\projects\\VRPN-Free-d.git\\install\\Release-x64\\"
            }
        );

        PublicAdditionalLibraries.AddRange
        (
            new string[]
            {
                "quatlib.lib",
                "vrpn.lib"
            }
        );

        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
