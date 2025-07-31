#pragma once

#include <Nigozi.h>

class EditorLayer : public Nigozi::Layer
{
public:
	EditorLayer(Nigozi::FrameBuffer* viewportBuffer);

	void OnUpdate(float timestep) override;
	void OnRender() override;
	void OnImGuiRender() override;

	inline ImVec2& GetViewportSize() { return m_viewportSize; }
private:
	Nigozi::SceneManager m_sceneManager;
	Nigozi::FrameBuffer* p_viewportBuffer;
	std::shared_ptr<Nigozi::Scene> m_scene;

	ImVec2 m_viewportSize;
};

