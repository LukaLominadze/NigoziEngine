project "%{PROJECT_NAME}"
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

    links { "%{NIGOZI_CSHARP_CORE_BIN}/Coral.Managed", "%{NIGOZI_CSHARP_CORE_BIN}/Nigozi-CSharpCore" }