workspace "NigoziEngine"
	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	architecture "x64"

	startproject "Kakali"

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

group "Editor"
	include "Kakali"
group ""
