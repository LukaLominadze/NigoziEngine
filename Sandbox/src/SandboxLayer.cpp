#include "SandboxLayer.h"

void SandboxLayer::OnAttach() {
	LOG("Hey! it's wooorking!");
}

void SandboxLayer::OnEvent(Nigozi::Event& event)
{
	Nigozi::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Nigozi::MouseButtonPressedEvent>(std::bind(&SandboxLayer::OnMouseButtonPressed, this, std::placeholders::_1));
}

void SandboxLayer::OnUpdate(float timestep)
{
	if (Nigozi::Input::IsKeyJustPressed(GLFW_KEY_SPACE)) {
		LOG("Space Just Pressed!");
		m_keystate = "GLFW_PRESS";
	}
	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_SPACE)) {
		m_keystate = "GLFW_REPEAT";
	}
	else if (Nigozi::Input::IsKeyReleased(GLFW_KEY_SPACE)) {
		LOG("Space Just Released");
		m_keystate = "GLFW_RELEASE";
	}
	else {
		m_keystate = "NONE";
	}
	m_mouseDelta = Nigozi::Input::GetMousePositionDelta();
	m_mousePosition = Nigozi::Input::GetMousePosition();
}

void SandboxLayer::OnImGuiRender()
{
	ImGui::Begin("Test Window!");

	ImGui::Text("Congratulations! Yo balls!");
	ImGui::Text("Mouse Delta");
	ImGui::Text((std::to_string(m_mouseDelta.first) + ", " + std::to_string(m_mouseDelta.second)).c_str());
	ImGui::Text("Mouse Position");
	ImGui::Text((std::to_string(m_mousePosition.first) + ", " + std::to_string(m_mousePosition.second)).c_str());
	ImGui::Text("Space key state");
	ImGui::Text(m_keystate.c_str());

	ImGui::End();
}

bool SandboxLayer::OnMouseButtonPressed(Nigozi::MouseButtonPressedEvent& event)
{
	if (event.GetButton() == GLFW_MOUSE_BUTTON_1) {
		LOG("Wow, you managed to click a button");
		return true;
	}
	return false;
}
