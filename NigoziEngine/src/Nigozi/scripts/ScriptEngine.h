#pragma once
#include "ngpch.h"

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
	private:
		static void LoadAssemblies(const std::filesystem::path& coralDir);
		static void UnloadAssemblies();
	private:
		static std::shared_ptr<SceneTree> s_currentScene;
	};
}

