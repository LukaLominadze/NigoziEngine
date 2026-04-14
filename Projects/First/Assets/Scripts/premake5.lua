include (path.join("..", "..", "..", "..", "Nigozi-CSharpCore", "premake5-dotnet.lua"))

project "First"
    language "C#"
    dotnetframework "net9.0"
    kind "SharedLib"
	clr "Unsafe"

    targetdir("../../Resources/Build")
    objdir("../../Resources/Intermediates")
	
    -- Don't specify architecture here. (see https://github.com/premake/premake-core/issues/1758)

	vsprops {
		AppendTargetFrameworkToOutputPath = "false",
		Nullable = "enable",
		CopyLocalLockFileAssemblies = "true",
		EnableDynamicLoading = "true",
	}

    files {
        "**.cs"
    }
    
    links { "Coral.Managed", "Nigozi-CSharpCore" }