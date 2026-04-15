include (path.join("..", "vendor", "Coral", "Premake", "CSExtensions.lua"))

workspace "Nigozi-CSharpCore"
	configurations { "Debug", "Release" }

	filter "configurations:Debug or configurations:Debug-AS"
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Distribution"
		optimize "Full"
		symbols "Off"

	include "../vendor/Coral/Coral.Managed"

	project "Nigozi-CSharpCore"
		kind "SharedLib"
		language "C#"
		dotnetframework "net9.0"
		clr "Unsafe"
		targetdir ("../Kakali/src/res/scripts")
		objdir ("../Kakali/src/res/scripts/bin-int")

		--linkAppReferences(false)

		links { "Coral.Managed" }

		propertytags {
			{ "AppendTargetFrameworkToOutputPath", "false" },
			{ "Nullable", "enable" },
		}

		files {
			"Source/**.cs",
			"Properties/**.cs"
		}
