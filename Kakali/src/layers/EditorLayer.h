#pragma once

#include <Nigozi.h>

class EditorLayer : public Nigozi::Layer
{
public:
	EditorLayer(Nigozi::FrameBuffer* viewportBuffer);

	void OnRender() override;
	void OnImGuiRender() override;

	inline ImVec2& GetViewportSize() { return m_viewportSize; }
private:
	Nigozi::FrameBuffer* p_viewportBuffer;

	ImVec2 m_viewportSize;
};

