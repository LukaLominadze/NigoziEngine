#pragma once

#include "ngpch.h"
#include "layers/Layer.h"

namespace Nigozi
{
	class Entity;
	class SceneManager;

	/*
		TODO: There probably should be different versions of a scene,
		for runtime and for the editor, since components behave differently
		in each
	*/
	class Scene : public Layer
	{
	public:
		Scene(SceneManager* sceneManager);
		~Scene();

		/*
			TODO: It would be good to have scene metadeta saved in a file
			and just have a LoadScene function, which would all the associated
			data to that scene. This way we also wouldn't have to create
			inherited scene classes to have different scenes
		*/
		void OnEvent(Event& event) override;
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

