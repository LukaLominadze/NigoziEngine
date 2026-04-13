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

	filter "language:C++ or language:C"
		architecture "x64"

	defines { "YAML_CPP_STATIC_DEFINE" }

	startproject "Kakali"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/glfw"
	include "vendor/glew"
	include "vendor/stb"
	include "vendor/yaml-cpp"
	include "vendor/imgui"
	include "vendor/nfd/build/Build-nfd-Only.lua"
	include "vendor/box2d"
	include "vendor/Coral/Coral.Native"
	include "vendor/Coral/Coral.Managed"
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
