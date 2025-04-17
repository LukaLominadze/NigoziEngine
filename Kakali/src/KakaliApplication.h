#pragma once

#include <Nigozi.h>
#include "layers/EditorLayer.h"
#include <layers/CameraControllerLayer.h>

class KakaliApplication : public Nigozi::Application
{
public:
	KakaliApplication(const Nigozi::ApplicationProps& props);
	~KakaliApplication();
protected:
	void OnRender() override;
private:
	CameraControllerLayer m_cameraLayer;
	Nigozi::FrameBuffer m_viewportBuffer;
	EditorLayer m_editorLayer;

	ImVec2 m_viewportSize, m_newViewportSize;
};

