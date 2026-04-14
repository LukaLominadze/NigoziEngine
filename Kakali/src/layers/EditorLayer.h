#pragma once

#include <Nigozi.h>
#include "EditorCamera.h"
#include "command/CommandQueue.h"
#include <new>
#include "EditorParmas.h"

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
	EditorLayer(const EditorParams& params, Nigozi::FrameBuffer* viewportBuffer);

	void OnAttach() override;
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
	void Hotkeys();

	void DockViewportWithMenuBar();
	void ShowSceneHierarchy();
	void DrawSceneHierarchyNode(Nigozi::Entity entity);
	void ShowInspector();
	void ShowAddNodeModal();
	void ShowViewport();

	void ShowCreateOrOpenProjectModal();

	enum EditValueInInspectorFlags 
	{
		NONE = 0,
		COMBO_BOX = 1 << 0
	};

	void UpdateRigidbodyTransform(Nigozi::Entity entity);
	void UpdateBoxCollider(Nigozi::Entity entity, glm::vec2 size);

	template<typename T>
	void EditValueInInspector(const std::function<void()>& imGuiFunction, const std::function<void(T)>& onEditedFunction, Nigozi::UUID uuid, T args, 
							  EditValueInInspectorFlags flags = EditValueInInspectorFlags::NONE)
	{
		if (m_editValueSelectionContext != m_selectionContext) {
			m_editValueSelectionContext = m_selectionContext;
			m_editValueData.clear();
		}
		static_assert(sizeof(args) <= 24, "[Editor Layer] Size of input values must be lower or equal to buffer!");
		imGuiFunction();
		if (ImGui::IsItemActivated()) {
			new (m_editValueData[uuid.GetUUID()]) T(args);
		}
		if (flags & EditValueInInspectorFlags::COMBO_BOX && ImGui::IsItemEdited()) {
			onEditedFunction(*(T*)m_editValueData[uuid.GetUUID()]);
		}
		else if (ImGui::IsItemDeactivatedAfterEdit()) {
			onEditedFunction(*(T*)m_editValueData[uuid.GetUUID()]);
		}
	}

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
	EditorParams m_editorParams;

	std::vector<std::shared_ptr<Nigozi::SceneTree>> m_sceneTreeContexts;
	std::shared_ptr<Nigozi::SceneTree> m_lastContext;
	std::shared_ptr<Nigozi::SceneTree> m_currentContext;
	CommandQueue m_commandQueue;

	std::unordered_map<uint64_t, char[24]> m_editValueData;

	Nigozi::FrameBuffer* p_viewportBuffer;
	EditorCamera m_editorCamera;

	Tool m_tool = Tool::SELECT;
	EditorState m_editorState = EditorState::EDIT;

	ImVec2 m_viewportSize;
	glm::vec2 m_windowPosition;
	glm::vec2 m_viewportPosition;
	bool m_viewportHovered = false;

	entt::entity m_selectionContext = entt::null;
	entt::entity m_movingSelectionContext = entt::null;

	entt::entity m_editValueSelectionContext = entt::null;

	glm::vec2 m_mouseOldPosition;
};

