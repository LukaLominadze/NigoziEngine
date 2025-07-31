#include "KakaliApplication.h"

KakaliApplication::KakaliApplication(const Nigozi::ApplicationProps& props)
	: Application(props), m_cameraLayer(props.Width / (float)props.Height),
	  m_viewportBuffer({ props.Width, props.Height }), 
	  m_editorLayer(&m_viewportBuffer)
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
	// Adjust the framebuffer size according to the GUI viewport
	// We do this before rendering to make sure there is no tearing 
	// or flashing on the screen
    ImVec2& viewportSize = m_editorLayer.GetViewportSize();
    if (*(glm::vec2*)&viewportSize != *(glm::vec2*)&m_viewportSize) {
        m_viewportSize = viewportSize;
        m_viewportBuffer.Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        m_cameraLayer.OnResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
    }

	// Draw everything onto the viewport
	m_viewportBuffer.Bind();

	Application::OnRender();

	m_viewportBuffer.Unbind();
}
