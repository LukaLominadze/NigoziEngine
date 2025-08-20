project "NigoziEngine"
	kind "StaticLib"
	staticruntime "off"

	language "C++"
	cppdialect "C++20"

	targetdir ("%{wks.location}/bin/" ..outputdir.. "/%{prj.name}")	
	objdir ("%{wks.location}/bin-int/" ..outputdir.. "/%{prj.name}")

	defines { "GLEW_STATIC" }
	
	files {
		"src/**.h",
		"src/**.cpp",
		"src/**.c",
		"src/ngpch.cpp",
		"src/Nigozi.h"
	}

	vpaths {
		["Source Files"] = "src/**.cpp",
		["Header Files"] = "src/**.h"
	}

	pchheader "ngpch.h"
	pchsource "ngpch.cpp"

	includedirs { "src",
				  "src/Nigozi",
				  "%{prj.location}/../dependencies/glew/include",
				  "%{prj.location}/../dependencies/glfw/include",
				  "%{prj.location}/../dependencies/stb",
				  "%{prj.location}/../dependencies/imgui",
				  "%{prj.location}/../dependencies/glm" }

	libdirs { "%{prj.location}/../dependencies/libs/GLFW",
			  "%{prj.location}/../dependencies/glew/lib/Release/x64",
			  "%{prj.location}/../dependencies/libs/STB",
			  "%{prj.location}/../dependencies/libs/ImGui",
			  "%{prj.location}/../dependencies/glew/lib" }
			  
	links { "GLEW",
		"GLFW",
		"STB",
		"ImGui" }

	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines { "NG_PLATFORM_WINDOWS" }
		
		links { "GLEW",
			"opengl32.lib",
			"GLFW",
			"STB",
			"ImGui" }
	
	filter "system:linux"
		systemversion "latest"
		
		defines { "GLFW_USE_X11", "_GLFW_X11", "NG_PLATFORM_LINUX" }
		
		links { "GL",
			"GLEW",
			"GLFW",
			"STB",
			"ImGui",
			"pthread",
        		"dl" }

	filter "configurations:Debug"
		
		defines { "_DEBUG" }
		
		architecture "x64"
		symbols "on"

	filter "configurations:Release"
		
		defines { "NDEBUG" }

		architecture "x64"
		optimize "on"
		
	filter "configurations:Distribution"
		
	defines { "DISTRIBUTION" }

	architecture "x64"
	optimize "on"
