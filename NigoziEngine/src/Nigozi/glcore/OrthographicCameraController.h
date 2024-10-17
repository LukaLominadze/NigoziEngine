#pragma once

#include "glcore/OrthographicCamera.h"
#include "events/Event.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

namespace Nigozi
{
	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		~OrthographicCameraController();

		inline const OrthographicCamera& GetCamera() const { return m_camera; }

		void OnEvent(Event& event);
		void OnUpdate(float timestep);
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);
	private:
		float m_aspectRatio;
		float m_zoom = 1.0f;
		OrthographicCamera m_camera;

		bool m_rotation = false;

		float m_cameraTranslationSpeed = 3.0f, m_cameraRotationSpeed = 180.0f;
	};
}
