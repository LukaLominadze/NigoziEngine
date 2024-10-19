#pragma once

#include <Nigozi.h>

class SandboxLayer : public Nigozi::Layer {
public:
	void OnAttach() override;
	void OnEvent(Nigozi::Event& event) override;
	void OnUpdate(float timestep) override;
	void OnImGuiRender() override;
private:
	bool OnMouseButtonPressed(Nigozi::MouseButtonPressedEvent& event);

	std::string m_keystate;
	std::pair<int, int> m_mouseDelta;
	std::pair<int, int> m_mousePosition;
};