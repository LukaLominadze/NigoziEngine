#include "ngpch.h"

#include "PerspectiveCameraController.h"
#include "core/Input.h"

namespace Nigozi
{
	PerspectiveCameraController::PerspectiveCameraController(float aspectRatio, float fov)
		: m_aspectRatio(aspectRatio), m_fov(fov), m_camera(m_fov, m_aspectRatio, 0.1f, 100.0f)
	{
	}

	PerspectiveCameraController::~PerspectiveCameraController()
	{
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void PerspectiveCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher = EventDispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&PerspectiveCameraController::OnMouseScrolled, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizedEvent>(std::bind(&PerspectiveCameraController::OnWindowResized, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&PerspectiveCameraController::OnMouseButtonPressed, this, std::placeholders::_1));
		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&PerspectiveCameraController::OnEscPressed, this, std::placeholders::_1));
	}

	void PerspectiveCameraController::OnUpdate(float timestep)
	{
		glm::vec3 cameraPosition = m_camera.GetPosition();
		glm::vec3 cameraRotation = m_camera.GetRotation();
		if (Input::IsKeyPressed(GLFW_KEY_A))
		{
			cameraPosition -= m_camera.GetRight() * m_cameraTranslationSpeed * timestep;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_D))
		{
			cameraPosition += m_camera.GetRight() * m_cameraTranslationSpeed * timestep;
		}

		if (Input::IsKeyPressed(GLFW_KEY_W))
		{
			cameraPosition += m_camera.GetFront() * m_cameraTranslationSpeed * timestep;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_S))
		{
			cameraPosition -= m_camera.GetFront() * m_cameraTranslationSpeed * timestep;
		}
		if (Input::IsKeyPressed(GLFW_KEY_SPACE)) {
			cameraPosition += m_camera.GetUp() * m_cameraTranslationSpeed * timestep;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
			cameraPosition -= m_camera.GetUp() * m_cameraTranslationSpeed * timestep;
		}

		std::pair<float, float> delta = Input::GetMousePositionDelta();

		if (m_mouseLocked) {
			float sensitivity = 0.1f;
			m_camera.SetPosition(cameraPosition);

			float yaw = m_camera.GetYaw();
			float pitch = m_camera.GetPitch();

			yaw += delta.first * sensitivity;
			pitch -= delta.second * sensitivity;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			m_camera.SetRotation(glm::vec3(pitch, yaw, 0.0f));
		}
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_fov -= e.GetOffsetY();
		m_fov = std::max(m_fov, 1.0f);
		m_fov = std::min(m_fov, 90.0f);
		m_camera.SetProjection(m_fov, m_aspectRatio, 0.1f, 100.0f);
		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizedEvent& e)
	{
		m_aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_camera.SetProjection(m_fov, m_aspectRatio, 0.1f, 100.0f);
		return false;
	}

	bool PerspectiveCameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == GLFW_MOUSE_BUTTON_1) {
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_mouseLocked = true;
		}
		return false;
	}

	bool PerspectiveCameraController::OnEscPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == GLFW_KEY_ESCAPE) {
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_mouseLocked = false;
		}
		return false;
	}
}
