#pragma once

#include <Nigozi.h>
#include "layers/EditorLayer.h"
#include "EditorParmas.h"

class KakaliApplication : public Nigozi::Application
{
public:
	KakaliApplication(const Nigozi::ApplicationProps& props, const EditorParams& params);
	~KakaliApplication();
protected:
	void OnRender() override;
private:
	Nigozi::FrameBuffer m_viewportBuffer;
	EditorLayer m_editorLayer;

	ImVec2 m_viewportSize, m_newViewportSize;
};

