#pragma once

#include "ngpch.h"
#include "layers/Layer.h"

namespace Nigozi
{
	class Entity;
	class SceneManager;

	class Scene : public Layer
	{
	public:
		Scene(SceneManager* sceneManager);
		~Scene();

		void OnUpdate(float timestep) override;
		void OnRender() override;
		void OnImGuiRender() override;

		Entity CreateEntity(const std::string& name, const std::string& tag);
		Entity TryGetEntityByTag(const std::string& tag = "");
		std::vector<Entity> TryGetEntitiesByTag(const std::string& tag = "");

		friend class Entity;
		entt::registry m_Registry;

		inline SceneManager* GetSceneManager() { return p_sceneManager; }
	private:
		SceneManager* p_sceneManager;
	};
}

