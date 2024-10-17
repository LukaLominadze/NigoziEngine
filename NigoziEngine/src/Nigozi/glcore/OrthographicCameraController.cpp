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
		if (Input::IsKeyPressed(GLFW_KEY_A))
		{
			cameraPosition.x -= cos(glm::radians(cameraRotation)) * m_cameraTranslationSpeed * timestep;
			cameraPosition.y -= sin(glm::radians(cameraRotation)) * m_cameraTranslationSpeed * timestep;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_D))
		{
			cameraPosition.x += cos(glm::radians(cameraRotation)) * m_cameraTranslationSpeed * timestep;
			cameraPosition.y += sin(glm::radians(cameraRotation)) * m_cameraTranslationSpeed * timestep;
		}

		if (Input::IsKeyPressed(GLFW_KEY_W))
		{
			cameraPosition.x += -sin(glm::radians(cameraRotation)) * m_cameraTranslationSpeed * timestep;
			cameraPosition.y += cos(glm::radians(cameraRotation)) * m_cameraTranslationSpeed * timestep;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_S))
		{
			cameraPosition.x -= -sin(glm::radians(cameraRotation)) * m_cameraTranslationSpeed * timestep;
			cameraPosition.y -= cos(glm::radians(cameraRotation)) * m_cameraTranslationSpeed * timestep;
		}

		m_camera.SetPosition(cameraPosition);

		m_cameraTranslationSpeed = m_zoom;
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

