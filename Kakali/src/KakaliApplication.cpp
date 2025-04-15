#include "KakaliApplication.h"

KakaliApplication::KakaliApplication(const Nigozi::ApplicationProps& props)
	: Application(props)
{
	PushOverlay(&m_editorLayer);

	LOG("Welcome to the Kakali editor!");
}

KakaliApplication::~KakaliApplication()
{
}
