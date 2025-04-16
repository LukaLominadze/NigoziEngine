#pragma once

#include <Nigozi.h>
#include "layers/EditorLayer.h"
#include <layers/CameraController.h>

class KakaliApplication : public Nigozi::Application
{
public:
	KakaliApplication(const Nigozi::ApplicationProps& props);
	~KakaliApplication();
protected:
	void OnRender() override;
private:
	CameraController m_cameraLayer;
	Nigozi::FrameBuffer m_framebuffer;
	EditorLayer m_editorLayer;

	ImVec2 m_viewportSize, m_newViewportSize;
};

