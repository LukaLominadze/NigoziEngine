project "MyProject"
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

    links { "E:/Luka/Dev/C++/NigoziEngine/Kakali/src/res/scripts/Coral.Managed", "E:/Luka/Dev/C++/NigoziEngine/Kakali/src/res/scripts/Nigozi-CSharpCore" }