#include "ngpch.h"

#include "OrthographicCameraController.h"
#include "core/Input.h"

namespace Nigozi
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_aspectRatio(aspectRatio), m_rotation(rotation), m_camera(-m_aspectRatio * m_zoom, m_aspectRatio* m_zoom, -m_zoom, m_zoom)
	{
	}

	OrthographicCameraController::~OrthographicCameraController()
	{
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher = EventDispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrthographicCameraController::OnMouseScrolled, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizedEvent>(std::bind(&OrthographicCameraController::OnWindowResized, this, std::placeholders::_1));
	}

	void OrthographicCameraController::OnUpdate(float timestep)
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

		if (Input::IsKeyPressed(GLFW_KEY_A))
		{
			cameraPosition.x -= cosRotation * m_cameraTranslationSpeed * timestep;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_D))
		{
			cameraPosition.x += cosRotation * m_cameraTranslationSpeed * timestep;
		}

		if (Input::IsKeyPressed(GLFW_KEY_W))
		{
			cameraPosition.y += m_cameraTranslationSpeed * timestep;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_S))
		{
			cameraPosition.y -= m_cameraTranslationSpeed * timestep;
		}

		if (Input::IsKeyPressed(GLFW_KEY_Q)) {
			cameraRotation += m_cameraRotationSpeed * timestep;
			m_camera.SetRotation(cameraRotation);
		}
		if (Input::IsKeyPressed(GLFW_KEY_E)) {
			cameraRotation -= m_cameraRotationSpeed * timestep;
			m_camera.SetRotation(cameraRotation);
		}

		m_camera.SetPosition(cameraPosition);

		m_cameraTranslationSpeed = 3 * m_zoom;
	}

	void OrthographicCameraController::OnRender()
	{
		m_camera.SetMVPMatrix();
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_zoom -= e.GetOffsetY() * 0.25f;
		m_zoom = std::max(m_zoom, 0.25f);
		m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& e)
	{
		m_aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_camera.SetProjection(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom);
		return false;
	}
}

