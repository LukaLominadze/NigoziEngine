#pragma once

#include <Nigozi.h>
#include "EditorCamera.h"

class EditorLayer : public Nigozi::Layer
{
public:
	EditorLayer(Nigozi::FrameBuffer* viewportBuffer);

	void OnEvent(Nigozi::Event& event) override;
	void OnUpdate(float timestep) override;
	void OnRender() override;
	void OnImGuiRender() override;

	inline ImVec2& GetViewportSize() { return m_viewportSize; }
private:
	bool OnMouseButtonPressed(Nigozi::MouseButtonPressedEvent& event);
	bool OnMouseButtonReleased(Nigozi::MouseButtonReleasedEvent& event);
	bool OnMouseMoved(Nigozi::MouseMovedEvent& event);
private:
	void DockViewportWithMenuBar();
	void ShowSceneHierarchy();
	void ShowInspector();
	void ShowAddComponentModal();

	template<typename T>
	void AddComponent(Nigozi::Entity& entity, bool selectedComponent) {
		if (selectedComponent &&
			!entity.HasComponent<T>()) {
			entity.AddComponent<T>();
		}
	}
private:
	Nigozi::SceneManager m_sceneManager;
	Nigozi::FrameBuffer* p_viewportBuffer;
	std::shared_ptr<Nigozi::Scene> m_scene;
	EditorCamera m_editorCamera;

	ImVec2 m_viewportSize;
	glm::vec2 m_windowPosition;
	glm::vec2 m_viewportPosition;
	bool m_viewportHovered = false;

	entt::entity m_selectionContext = entt::null;
	entt::entity m_movingSelectionContext = entt::null;

	glm::vec2 m_mouseOldPosition;
};

