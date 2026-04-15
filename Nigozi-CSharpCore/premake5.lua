include (path.join("..", "vendor", "Coral", "Premake", "CSExtensions.lua"))
include (path.join("..", "vendor", "Coral", "Coral.Managed"))

project "Nigozi-CSharpCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "net9.0"
	clr "Unsafe"
	targetdir "%{wks.location}/Kakali/src/res/scripts"
	objdir "%{wks.location}/Kakali/src/res/scripts/bin-int"

	configmap { ["Distribution"] = "Release" }

	links {
		"Coral.Managed"
	}

	propertytags {
		{ "AppendTargetFrameworkToOutputPath", "false" },
		{ "Nullable", "enable" },
	}

	files {
		"Source/**.cs",
		"Properties/**.cs"
	}