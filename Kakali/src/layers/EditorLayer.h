#pragma once

#include <Nigozi.h>
#include "EditorCamera.h"
#include "command/CommandQueue.h"

enum class Tool
{
	NONE = 0,
	SELECT,
	MOVE,
	ROTATE
};

enum class EditorState 
{
	EDIT = 0,
	PLAY,
	PAUSE
};

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
	bool OnSelectMouseButtonPressed(Nigozi::MouseButtonPressedEvent& event);
	bool OnMouseMoved(Nigozi::MouseMovedEvent& event);
	bool OnRotateMouseMoved(Nigozi::MouseMovedEvent& event);
	bool OnMouseButtonReleased(Nigozi::MouseButtonReleasedEvent& event);
	bool OnMoveMouseButtonPressed(Nigozi::MouseButtonPressedEvent& event);
private:
	void DockViewportWithMenuBar();
	void ShowSceneHierarchy();
	void DrawSceneHierarchyNode(Nigozi::Entity entity);
	void ShowInspector();
	void ShowAddNodeModal();
	void ShowViewport();

	void CloseSceneTab(const std::shared_ptr<Nigozi::SceneTree>& sceneContext);

	bool SaveAllScenes();
	bool SaveScene(const std::shared_ptr<Nigozi::SceneTree>& context);
	bool SaveCurrentScene();
	bool SaveCurrentSceneAs();
	void LoadScene();

	template<typename T>
	void AddComponent(Nigozi::Entity& entity, bool selectedComponent) {
		if (selectedComponent &&
			!entity.HasComponent<T>()) {
			entity.AddComponent<T>();
		}
	}
private:
	static bool s_showDemoWindow;
private:
	std::vector<std::shared_ptr<Nigozi::SceneTree>> m_sceneTreeContexts;
	std::shared_ptr<Nigozi::SceneTree> m_lastContext;
	std::shared_ptr<Nigozi::SceneTree> m_currentContext;

	Nigozi::FrameBuffer* p_viewportBuffer;
	EditorCamera m_editorCamera;

	Tool m_tool = Tool::SELECT;
	EditorState m_editorState = EditorState::EDIT;

	CommandQueue m_commandQueue;

	ImVec2 m_viewportSize;
	glm::vec2 m_windowPosition;
	glm::vec2 m_viewportPosition;
	bool m_viewportHovered = false;

	entt::entity m_selectionContext = entt::null;
	entt::entity m_movingSelectionContext = entt::null;

	glm::vec2 m_mouseOldPosition;
};

