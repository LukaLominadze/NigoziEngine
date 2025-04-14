project "Sandbox"
	kind "ConsoleApp"
	staticruntime "off"

	language "C++"
	cppdialect "C++20"

	targetdir ("%{wks.location}/bin/" ..outputdir.. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" ..outputdir.. "/%{prj.name}")

	defines { "GLEW_STATIC" }
	
	files {
		"src/**.h",
		"src/**.cpp"
	}

	vpaths {
		["Source Files"] = "src/**.cpp",
		["Header Files"] = "src/**.h"
	}

	includedirs {  "src",
				  "%{wks.location}/NigoziEngine/src",
				  "%{wks.location}/NigoziEngine/src/Nigozi",
				  "%{wks.location}/dependencies/glew/include",
				  "%{wks.location}/dependencies/glfw/include",
				  "%{wks.location}/dependencies/stb",
				  "%{wks.location}/dependencies/imgui",
				  "%{wks.location}/dependencies/glm" }

	libdirs { "%{wks.location}/dependencies/libs/GLFW",
			  "%{wks.location}/dependencies/glew/lib/Release/x64",
			  "%{wks.location}/dependencies/libs/STB",
			  "%{wks.location}/dependencies/libs/ImGui",
			  "%{wks.location}/dependencies/glew/lib" }

	links {
		"NigoziEngine",
		"GLEW",
		"GLFW",
		"STB",
		"ImGui",
		 }

	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

	postbuildcommands {
		"{COPYDIR} %{wks.location}/NigoziEngine/src/Nigozi/res %{prj.location}/src/Nigozi/res"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		
		defines { "NG_PLATFORM_WINDOWS" }
		
		links { "opengl32.lib" }
	
	filter "system:linux"
		cppdialect "C++20"
		systemversion "latest"
		
		defines { "GLFW_USE_X11", "NG_PLATFORM_LINUX" }
		
		links { "GL",
			"pthread",
        		"dl" }

	filter "configurations:Debug"
		
		defines { "_DEBUG", "_CONSOLE" }
		
		architecture "x64"
		symbols "on"

	filter "configurations:Release"
		
		defines { "NDEBUG", "_CONSOLE" }

		architecture "x64"
		optimize "on"

	filter "configurations:Distribution"
		
		kind "WindowedApp"

		defines { "DISTRIBUTION" }
	
		architecture "x64"
		optimize "on"
