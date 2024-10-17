workspace "NigoziEngine"
	configurations {
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	architecture "x64"

	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "dependencies/glfw"
	include "dependencies/glew"
	include "dependencies/stb"
	include "dependencies/imgui"
group ""

group "Core"
	include "NigoziEngine"
group ""

group "Misc"
	include "Sandbox"
group ""
