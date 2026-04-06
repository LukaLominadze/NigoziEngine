#pragma once

#include <Nigozi.h>

class EditorCamera {
public:
	EditorCamera(float aspectRatio, float zoom = 5.0f, bool rotation = false);
	~EditorCamera();

	inline Nigozi::OrthographicCamera& GetCamera() { return m_camera; }

	void OnEvent(Nigozi::Event& event);
	void OnUpdate(float timestep);
	void OnRender();
	void OnResize(uint32_t width, uint32_t height);
private:
	bool OnMouseScrolled(Nigozi::MouseScrolledEvent& e);
	bool OnMouseMoved(Nigozi::MouseMovedEvent& e);
private:
	float m_aspectRatio = 1.0f;
	float m_zoom = 1.0f;

	bool m_rotation = false;

	float m_cameraTranslationSpeed = 3.0f, m_cameraRotationSpeed = 180.0f;

	glm::vec2 m_viewportSize;
	
	Nigozi::OrthographicCamera m_camera;
};

