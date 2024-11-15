#include "SandboxLayer.h"

void SandboxLayer::OnAttach() {
	LOG("Hey! it's wooorking!");
	Nigozi::Renderer2D::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	m_luigiTexture = std::make_shared<Nigozi::Texture>("src/Nigozi/res/textures/luigi.png");
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
	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_LEFT)) {
		m_luigiPosition.x -= 4.0f * timestep;
	}
	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_RIGHT)) {
		m_luigiPosition.x += 4.0f * timestep;
	}
	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_DOWN)) {
		m_luigiPosition.y -= 4.0f * timestep;
	}
	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_UP)) {
		m_luigiPosition.y += 4.0f * timestep;
	}
	m_mouseDelta = Nigozi::Input::GetMousePositionDelta();
	m_mousePosition = Nigozi::Input::GetMousePosition();
}

void SandboxLayer::OnRender()
{
	float increment = 1 / 200.0f;
	glm::vec4 color(increment, increment * increment, 1.0f, 1.0f);

	for (float i = 1 / 200.0f; i < 1; i += increment) {
		for (float j = 1 / 200.0f; j < 1; j += increment) {
			Nigozi::Renderer2D::DrawQuad({ i, j, -1.0f }, { increment, increment }, nullptr, color);
			color.z -= (increment * increment);
			color.y = i * j;
		}
		color.x += increment;
	}
	Nigozi::Renderer2D::DrawQuad(m_luigiPosition, { 0.2f, 0.2f }, m_luigiTexture, { 1.0f, 1.0f, 1.0f, 1.0f });
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
	ImGui::Text("Draw Calls");
	ImGui::Text(std::to_string(Nigozi::Renderer2D::GetData()->DrawCalls).c_str());
	ImGui::Text("Quad Count");
	ImGui::Text(std::to_string(Nigozi::Renderer2D::GetData()->QuadCount).c_str());

	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}

bool SandboxLayer::OnMouseButtonPressed(Nigozi::MouseButtonPressedEvent& event)
{
	if (event.GetButton() == GLFW_MOUSE_BUTTON_1) {
		LOG("Wow, you managed to click a button");
		return true;
	}
	return false;
}
