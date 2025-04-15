#pragma once

#include <Nigozi.h>
#include "layers/EditorLayer.h"

class KakaliApplication : public Nigozi::Application
{
public:
	KakaliApplication(const Nigozi::ApplicationProps& props);
	~KakaliApplication();

protected:
	void OnRender() override;
private:
	Nigozi::OrthographicCameraController m_cameraLayer;
	Nigozi::FrameBuffer m_framebuffer;
	EditorLayer m_editorLayer;
};

