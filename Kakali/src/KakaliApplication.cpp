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
    ImVec2& viewportSize = m_editorLayer.GetViewportSize();
    if (*(glm::vec2*)&viewportSize != *(glm::vec2*)&m_viewportSize) {
        m_viewportSize = viewportSize;
        m_framebuffer.Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        m_framebuffer.Invalidate();
        m_cameraLayer.OnResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
    }

	m_framebuffer.Bind();

	Application::OnRender();

	m_framebuffer.Unbind();
}
