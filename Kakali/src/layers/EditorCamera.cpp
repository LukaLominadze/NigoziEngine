#include "EditorCamera.h"

EditorCamera::EditorCamera(float aspectRatio, bool rotation)
	:m_aspectRatio(aspectRatio), m_rotation(rotation), m_camera(-m_aspectRatio * m_zoom, m_aspectRatio* m_zoom, -m_zoom, m_zoom)
{
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::OnEvent(Nigozi::Event& event)
{
	Nigozi::EventDispatcher dispatcher = Nigozi::EventDispatcher(event);
	dispatcher.Dispatch<Nigozi::MouseScrolledEvent>(std::bind(&EditorCamera::OnMouseScrolled, this, std::placeholders::_1));
	dispatcher.Dispatch<Nigozi::MouseMovedEvent>(std::bind(&EditorCamera::OnMouseMoved, this, std::placeholders::_1));
}

void EditorCamera::OnUpdate(float timestep)
{
	if (Nigozi::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_3)) {
		glm::vec2 delta = Nigozi::Input::GetMousePositionDelta();
		glm::vec2 current = Nigozi::Input::GetMousePosition();
		glm::vec2 last = current - delta;
	
		glm::vec2 currentWorld = m_camera.GetMousePositionWorldSpace(current, m_viewportSize);
		glm::vec2 lastWorld = m_camera.GetMousePositionWorldSpace(last, m_viewportSize);
	
		glm::vec2 deltaWorld = currentWorld - lastWorld;
		glm::vec3 cameraPosition = m_camera.GetPosition();
		glm::vec3 newCameraPos = glm::vec3(cameraPosition.x - deltaWorld.x,
										   cameraPosition.y - deltaWorld.y, cameraPosition.z);
		m_camera.SetPosition(newCameraPos);
	}

	m_cameraTranslationSpeed = 3 * m_zoom;
}

void EditorCamera::OnRender()
{
	m_camera.SetMVPMatrix();
}

void EditorCamera::OnResize(uint32_t width, uint32_t height)
{
	m_viewportSize = glm::vec2(width, height);
	m_aspectRatio = (float)width / (float)height;
	m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
	GLCall(glViewport(0, 0, width, height));
}

bool EditorCamera::OnMouseScrolled(Nigozi::MouseScrolledEvent& e)
{
	m_zoom -= e.GetOffsetY() * 0.25f;
	m_zoom = std::max(m_zoom, 0.25f);
	m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
	return false;
}

bool EditorCamera::OnMouseMoved(Nigozi::MouseMovedEvent& e)
{
	return false;
}


