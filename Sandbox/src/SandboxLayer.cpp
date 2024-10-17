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

}

void SandboxLayer::OnImGuiRender()
{
	ImGui::Begin("Test Window!");

	ImGui::Text("Congratulations! Yo balls!");

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
