include "../vendor/Coral/Premake/DebuggerTypeExtension.lua"

project "Kakali"
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
				  "%{wks.location}/vendor/glew/include",
				  "%{wks.location}/vendor/glfw/include",
				  "%{wks.location}/vendor/stb",
				  "%{wks.location}/vendor/yaml-cpp/include",
				  "%{wks.location}/vendor/imgui",
				  "%{wks.location}/vendor/glm",
				  "%{wks.location}/vendor/nfd/src/include",
				  "%{wks.location}/vendor/box2d/include",
				  "%{prj.location}/../vendor/spdlog/include",
				  "%{wks.location}/vendor/Coral/Coral.Native/Include" }

	links {
		"box2d",
		"yaml-cpp",
		"nfd",
		"Coral.Native",
		"NigoziEngine",
		"GLEW",
		"GLFW",
		"STB",
		"ImGui"
		 }

	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

	filter "toolset:msc*"
  	buildoptions { "/utf-8" }

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		
		defines { "NG_PLATFORM_WINDOWS" }
		
		links { "opengl32.lib" }

		postbuildcommands {
			"{COPYDIR} %{wks.location}/NigoziEngine/src/Nigozi/res %{prj.location}/src/Nigozi/res",
			"{MKDIR} %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/src/Nigozi",
			"{COPYDIR} %{prj.location}/src/Nigozi %{wks.location}/bin/" ..outputdir.. "/%{prj.name}/src/Nigozi",
			"{COPYDIR} %{prj.location}/src/res %{wks.location}/bin/" ..outputdir.. "/%{prj.name}/src/res",
			"{COPYDIR} %{prj.location}/kakali-small.png %{wks.location}/bin/" ..outputdir.. "/%{prj.name}/",
			"{COPYDIR} %{prj.location}/imgui.ini %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/",
			"{COPYDIR} %{prj.location}/premake5.exe %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/",

			"{MKDIR} %{wks.location}/bin/" ..outputdir.. "/%{prj.name}/src/project",
			"{COPYDIR} %{prj.location}/src/project/premake5-workspace.lua %{wks.location}/bin/" ..outputdir.. "/%{prj.name}/src/project/",
			"{COPYDIR} %{prj.location}/src/project/premake5-project.lua %{wks.location}/bin/" ..outputdir.. "/%{prj.name}/src/project/",

			"{COPYDIR} %{wks.location}/vendor/Coral/Build/%{cfg.buildcfg} %{prj.location}/src/res/scripts/"
		}

		filter "configurations:Distribution"
		
			kind "WindowedApp"
	
	filter "system:linux"
		cppdialect "C++20"
		systemversion "latest"
		
		defines { "GLFW_USE_X11", "_GLFW_X11", "NG_PLATFORM_LINUX" }
		
		links { "GL",
			"pthread",
        	"dl" }
		
		postbuildcommands {
			"{COPYDIR} %{wks.location}/NigoziEngine/src/Nigozi/res %{prj.location}/src/Nigozi",
			"{MKDIR} %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/src",
			"{COPYDIR} %{prj.location}/src/Nigozi %{wks.location}/bin/" ..outputdir.. "/%{prj.name}/src",
			"{COPYDIR} %{prj.location}/src/res %{wks.location}/bin/" ..outputdir.. "/%{prj.name}/src",
			"{COPYDIR} %{prj.location}/kakali-small.png %{wks.location}/bin/" ..outputdir.. "/%{prj.name}/",
			"{COPYDIR} %{prj.location}/imgui.ini %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/",
			"{COPYDIR} %{prj.location}/premake5.exe %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/",

			"{COPYDIR} %{wks.location}/vendor/Coral/Build/%{cfg.buildcfg} %{prj.location}/src/res/scripts/"
		}

	filter {"system:linux", "options:linux_backend=gtk3"}

		buildoptions {"`pkg-config --cflags gtk+-3.0`"}
		linkoptions  { "`pkg-config --libs gtk+-3.0`" }
		links { "gtk-3", "gobject-2.0", "glib-2.0" }

	filter "configurations:Debug"
		
		defines { "_DEBUG", "_CONSOLE" }
		
		architecture "x64"
		symbols "on"

	filter "configurations:Release"
		
		defines { "NDEBUG", "_CONSOLE" }

		architecture "x64"
		optimize "on"

	filter "configurations:Distribution"

		defines { "DISTRIBUTION" }
	
		architecture "x64"
		optimize "on"
