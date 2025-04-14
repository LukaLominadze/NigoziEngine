#pragma once

#include <Nigozi.h>

class SandboxLayer : public Nigozi::Layer {
public:
	void OnAttach() override;
	void OnEvent(Nigozi::Event& event) override;
	void OnUpdate(float timestep) override;
	void OnRender() override;
	void OnImGuiRender() override;
private:
	bool OnMouseButtonPressed(Nigozi::MouseButtonPressedEvent& event);
	bool OnFullscreenToggle(Nigozi::KeyPressedEvent& event);

	Nigozi::ParticleSystem m_particleSystem;

	std::shared_ptr<Nigozi::Texture> m_luigiTexture;
	std::shared_ptr<Nigozi::SubTexture> m_playerTexture;

	std::string m_keystate;
	std::pair<int, int> m_mouseDelta;
	std::pair<int, int> m_mousePosition;

	glm::vec3 m_luigiPosition;

	float m_elapsedTime = 0.0f;
	uint32_t m_slotX = 0;
};