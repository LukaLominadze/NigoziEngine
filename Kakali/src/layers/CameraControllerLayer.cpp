#include "CameraControllerLayer.h"

CameraControllerLayer::CameraControllerLayer(float aspectRatio, bool rotation)
	:m_aspectRatio(aspectRatio), m_rotation(rotation), m_camera(-m_aspectRatio * m_zoom, m_aspectRatio* m_zoom, -m_zoom, m_zoom)
{
}

CameraControllerLayer::~CameraControllerLayer()
{
}

void CameraControllerLayer::OnEvent(Nigozi::Event& event)
{
	Nigozi::EventDispatcher dispatcher = Nigozi::EventDispatcher(event);
	dispatcher.Dispatch<Nigozi::MouseScrolledEvent>(std::bind(&CameraControllerLayer::OnMouseScrolled, this, std::placeholders::_1));
}

void CameraControllerLayer::OnUpdate(float timestep)
{
	glm::vec3 cameraPosition = m_camera.GetPosition();
	float cameraRotation = m_camera.GetRotation();

	float cosRotation = cos(glm::radians(cameraRotation));
	float sinRotation = sin(glm::radians(cameraRotation));

	if (sinRotation == 0) {
		// disregard rotation
		cosRotation = 1;
		sinRotation = 1;
	}

	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_A))
	{
		cameraPosition.x -= cosRotation * m_cameraTranslationSpeed * timestep;
	}
	else if (Nigozi::Input::IsKeyPressed(GLFW_KEY_D))
	{
		cameraPosition.x += cosRotation * m_cameraTranslationSpeed * timestep;
	}

	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_W))
	{
		cameraPosition.y += m_cameraTranslationSpeed * timestep;
	}
	else if (Nigozi::Input::IsKeyPressed(GLFW_KEY_S))
	{
		cameraPosition.y -= m_cameraTranslationSpeed * timestep;
	}

	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_Q)) {
		cameraRotation += m_cameraRotationSpeed * timestep;
		m_camera.SetRotation(cameraRotation);
	}
	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_E)) {
		cameraRotation -= m_cameraRotationSpeed * timestep;
		m_camera.SetRotation(cameraRotation);
	}
	if (Nigozi::Input::IsKeyJustPressed(GLFW_KEY_R)) {
		m_camera.SetRotation(0);
	}

	m_camera.SetPosition(cameraPosition);

	m_cameraTranslationSpeed = 3 * m_zoom;
}

void CameraControllerLayer::OnRender()
{
	m_camera.SetMVPMatrix();
}

void CameraControllerLayer::OnResize(uint32_t width, uint32_t height)
{
	m_aspectRatio = (float)width / (float)height;
	m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
	GLCall(glViewport(0, 0, width, height));
}

bool CameraControllerLayer::OnMouseScrolled(Nigozi::MouseScrolledEvent& e)
{
	m_zoom -= e.GetOffsetY() * 0.25f;
	m_zoom = std::max(m_zoom, 0.25f);
	m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
	return false;
}

