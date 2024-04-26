workspace "NigoziEngine"
	configurations {
		"Debug",
		"Release"
	}

	architecture "x64"

	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "NigoziEngine"
	location "NigoziEngine"
	kind "StaticLib"
	staticruntime "on"

	language "C++"
	cppdialect "C++20"

	targetdir ("bin/" ..outputdir.. "/%{prj.name}")
	objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

	defines { "NG_PLATFORM_WINDOWS", "NG_BUILD_DLL" }
	
	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/ngpch.cpp"
	}

	vpaths {
		["Source Files"] = "%{prj.name}/src/**.cpp",
		["Header Files"] = "%{prj.name}/src/**.h"
	}

	pchheader "ngpch.h"
	pchsource "ngpch.cpp"

	includedirs { "%{prj.name}/src",
				  "%{wks.location}/vendor/SDL2-2.30.0/include", }
	libdirs { "%{wks.location}/vendor/SDL2-2.30.0/lib/x64/" }

	links { "SDL2.lib",
			"SDL2main.lib",
			"SDL2_image.lib" }

	configurations {
		"Debug",
		"Release"
	}

	filter "system:windows"
		systemversion "latest"

		buildoptions { "/Yc\"ngpch.h\"" }

	filter "configurations:Debug"
		
		defines { "_DEBUG" }
		
		architecture "x64"
		symbols "on"

	filter "configurations:Release"
		
		defines { "NDEBUG" }

		architecture "x64"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	staticruntime "on"

	language "C++"
	cppdialect "C++20"

	targetdir ("bin/" ..outputdir.. "/%{prj.name}")
	objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

	defines { "NG_PLATFORM_WINDOWS", "_CONSOLE" }
	
	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	vpaths {
		["Source Files"] = "%{prj.name}/src/**.cpp",
		["Header Files"] = "%{prj.name}/src/**.h"
	}

	includedirs { "%{prj.name}/src/",
				  "%{wks.location}/vendor/SDL2-2.30.0/include",
				  "%{wks.location}/NigoziEngine/src"}
	libdirs { "%{wks.location}/vendor/SDL2-2.30.0/lib/x64/" }

	links { "SDL2.lib",
			"SDL2main.lib",
			"SDL2_image.lib" }

	configurations {
		"Debug",
		"Release"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "on"
		systemversion "latest"

		postbuildcommands {
			"{COPYFILE} %[%{wks.location}vendor/SDL2-2.30.0/lib/x64/SDL2.dll] %[%{wks.location}bin/" ..outputdir.. "/%{prj.name}/]",
			"{COPYFILE} %[%{wks.location}vendor/SDL2-2.30.0/lib/x64/SDL2_image.dll] %[%{wks.location}bin/" ..outputdir.. "/%{prj.name}/]"
		}

	filter "configurations:Debug"
		
		defines { "_DEBUG" }
		
		architecture "x64"
		symbols "on"

	filter "configurations:Release"
		
		defines { "NDEBUG" }

		architecture "x64"
		optimize "on"