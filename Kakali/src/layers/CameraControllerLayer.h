#pragma once

#include <Nigozi.h>

class CameraControllerLayer : public Nigozi::Layer {
public:
	CameraControllerLayer(float aspectRatio, bool rotation = false);
	~CameraControllerLayer();

	inline const Nigozi::OrthographicCamera& GetCamera() const { return m_camera; }

	void OnEvent(Nigozi::Event& event) override;
	void OnUpdate(float timestep) override;
	void OnRender() override;
	void OnResize(uint32_t width, uint32_t height);
private:
	bool OnMouseScrolled(Nigozi::MouseScrolledEvent& e);
private:
	float m_aspectRatio;
	float m_zoom = 1.0f;
	Nigozi::OrthographicCamera m_camera;

	bool m_rotation = false;

	float m_cameraTranslationSpeed = 3.0f, m_cameraRotationSpeed = 180.0f;
};

