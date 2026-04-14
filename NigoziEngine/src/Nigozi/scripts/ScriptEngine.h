#pragma once
#include "ngpch.h"

#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>

#include "scene/SceneTree.h"

namespace Nigozi
{
	static class ScriptEngine
	{
	public:
		static void Initialize(const std::filesystem::path& coralDir);
		static void Deinitialize();

		static inline std::shared_ptr<SceneTree> GetCurrentSceneTree() { return s_currentScene; }
		static inline void SetCurrentSceneTree(const std::shared_ptr<SceneTree>& sceneTree) { s_currentScene = sceneTree; }

		static void LoadProjectAssembly();
		static void ReloadAssemblies();

		static void StartRuntime();
		static void EndRuntime();
	private:
		static void LoadHostInstance();
		static void LoadCoreAssembly();
		static void UnloadCoreAssembly();
	private:
		static std::shared_ptr<SceneTree> s_currentScene;

		static Coral::HostInstance s_hostInstance;
		static Coral::AssemblyLoadContext s_loadContext;
		static Coral::ManagedAssembly* sp_coreAssembly;
		static Coral::ManagedAssembly* sp_projectAssembly;

		static std::filesystem::path s_coralDir;
	};
}

