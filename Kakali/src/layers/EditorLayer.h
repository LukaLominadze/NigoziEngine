#pragma once

#include <Nigozi.h>

class EditorLayer : public Nigozi::Layer
{
public:
	EditorLayer(Nigozi::FrameBuffer* viewportBuffer);

	void OnRender() override;
	void OnImGuiRender() override;
private:
	Nigozi::FrameBuffer* p_viewportBuffer;
};

