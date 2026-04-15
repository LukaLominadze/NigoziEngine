workspace "%{PROJECT_NAME}"
	configurations { "Debug", "Release" }

	filter "configurations:Debug or configurations:Debug-AS"
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

	project "SolutionItems"
        kind "None"
        files {
            ".editorconfig"
        }

    include("Assets/Scripts")