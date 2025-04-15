#pragma once

#include <Nigozi.h>
#include "layers/EditorLayer.h"

class KakaliApplication : public Nigozi::Application
{
public:
	KakaliApplication(const Nigozi::ApplicationProps& props);
	~KakaliApplication();
private:
	EditorLayer m_editorLayer;
};

