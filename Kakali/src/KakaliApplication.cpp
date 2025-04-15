#include "KakaliApplication.h"

KakaliApplication::KakaliApplication(const Nigozi::ApplicationProps& props)
	: Application(props), m_cameraLayer(props.Width / (float)props.Height),
	  m_framebuffer({ props.Width, props.Height }), 
	  m_editorLayer(&m_framebuffer)
{
	PushOverlay(&m_editorLayer);
	PushLayer(&m_cameraLayer);

	LOG("Welcome to the Kakali editor!");
}

KakaliApplication::~KakaliApplication()
{
	
}

void KakaliApplication::OnRender()
{
	m_framebuffer.Bind();

	Nigozi::Renderer2D::SetClearColor(0.6f, 0.4f, 0.0f, 1.0f);
	Application::OnRender();

	m_framebuffer.Unbind();
}
