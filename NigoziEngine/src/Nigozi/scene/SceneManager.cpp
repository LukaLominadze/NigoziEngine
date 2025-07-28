#include "ngpch.h"
#include "SceneManager.h"

namespace Nigozi
{
	SceneManager::SceneManager(const std::string& name, std::function<std::shared_ptr<Scene>()>&& func)
	{
		m_sceneInstantiators[name] = func;
		m_currentScene.first = name;
	}

	SceneManager::~SceneManager()
	{

	}

	void SceneManager::OnAttach()
	{
		m_currentScene.second = m_sceneInstantiators.at(m_currentScene.first)();
		m_currentScene.second->OnAttach();
	}

	void SceneManager::OnEvent(Event& event)
	{
		m_currentScene.second->OnEvent(event);
	}

	void SceneManager::OnUpdate(float timestep)
	{
		m_currentScene.second->OnUpdate(timestep);
	}

	void SceneManager::OnRender()
	{
		m_currentScene.second->OnRender();
	}

	void SceneManager::OnImGuiRender()
	{
		m_currentScene.second->OnImGuiRender();
	}

	void SceneManager::AddSceneToMap(const std::string& name, std::function<std::shared_ptr<Scene>()>&& func)
	{
		if (m_sceneInstantiators.contains(name)) {
			return;
		}
		m_sceneInstantiators[name] = func;
	}

	void SceneManager::LoadScene(const std::string& name)
	{
		if (!m_sceneInstantiators.contains(name)) {
			return;
		}
		if (m_currentScene.first == name) {
			return;
		}
		m_currentScene.second.reset();
		m_currentScene.first = name;
		m_currentScene.second = m_sceneInstantiators.at(name)();
		m_currentScene.second->OnAttach();
	}

	void SceneManager::LoadCurrentScene()
	{
		m_currentScene.second.reset();
		m_currentScene.second = m_sceneInstantiators.at(m_currentScene.first)();
		m_currentScene.second->OnAttach();
	}

	void SceneManager::UnloadScene(const std::string& name)
	{
		if (m_currentScene.first == name) {
			return;
		}
		m_currentScene.second.reset();
		m_currentScene.first = "";
	}
}