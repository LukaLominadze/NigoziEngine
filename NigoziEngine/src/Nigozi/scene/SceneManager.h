#pragma once

#include "ngpch.h"
#include "layers/Layer.h"
#include "Scene.h"

namespace Nigozi
{
	class SceneManager : public Layer
	{
	public:
		SceneManager() = default;
		SceneManager(const std::string& name, std::function<std::shared_ptr<Scene>()>&& func);
		~SceneManager();

		virtual void OnAttach() override;
		virtual void OnEvent(Event& event) override;
		virtual void OnUpdate(float timestep) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

		// TODO: Add ability to have multiple scenes at once
		// NOTE: The heap allocation of scenes is addres in Scene.h
		void AddSceneToMap(const std::string& name, std::function<std::shared_ptr<Scene>()>&& func);
		void LoadScene(const std::string& name);
		void LoadCurrentScene();
		void UnloadScene(const std::string& name);

		inline std::shared_ptr<Scene>& GetCurrentScene() { return m_currentScene.second; }
	private:
		friend class Scene;
		std::map<std::string, std::function<std::shared_ptr<Scene>()>> m_sceneInstantiators;
		std::pair<std::string, std::shared_ptr<Scene>> m_currentScene;
	};
}

