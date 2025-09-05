#include "SandboxLayer.h"

SandboxLayer::SandboxLayer(Nigozi::OrthographicCameraController* cameraLayer)
{
	p_cameraLayer = cameraLayer;
	p_audio = Nigozi::AudioEngine::LoadAudioFromFile("src/res/papyrus.mp3");
	p_audio->SetVolume(m_audioVolume);
}

void SandboxLayer::OnAttach() {
	NG_CLIENT_LOG_INFO("Hey! it's wooorking!");
	Nigozi::Renderer2D::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	m_luigiTexture = std::make_shared<Nigozi::Texture>("src/Nigozi/res/textures/luigi.png");
	std::shared_ptr<Nigozi::Texture> ref = std::make_shared<Nigozi::Texture>("src/res/Player.png");
	m_playerTexture = std::make_shared<Nigozi::SubTexture>(ref, glm::vec2{ 16, 16 }, 0, 0);
}

void SandboxLayer::OnEvent(Nigozi::Event& event)
{
	Nigozi::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Nigozi::MouseButtonPressedEvent>(std::bind(&SandboxLayer::OnMouseButtonPressed, this, std::placeholders::_1));
	dispatcher.Dispatch<Nigozi::KeyPressedEvent>(std::bind(&SandboxLayer::OnFullscreenToggle, this, std::placeholders::_1));
}

void SandboxLayer::OnUpdate(float timestep)
{
	p_audio->Update();
	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_P)) {
		if (!p_audio->IsPlaying()) {
			p_audio->Play();
		}
	}
	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_EQUAL)) {
		m_audioVolume += 12.0f * timestep;
		p_audio->SetVolume(m_audioVolume);
	}
	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_MINUS)) {
		m_audioVolume -= 12.0f * timestep;
		p_audio->SetVolume(m_audioVolume);
	}

	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_P)) {
		if (!p_audio->IsPlaying()) {
			p_audio->Play();
		}
	}

	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_SPACE)) {
		m_keystate = "GLFW_REPEAT";
	}
	else if (Nigozi::Input::IsKeyReleased(GLFW_KEY_SPACE)) {
		m_keystate = "GLFW_RELEASE";
	}
	else {
		m_keystate = "NONE";
	}
	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_LEFT)) {
		m_luigiPosition.x -= 2.0f * timestep;
	}
	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_RIGHT)) {
		m_luigiPosition.x += 2.0f * timestep;
	}
	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_DOWN)) {
		m_luigiPosition.y -= 2.0f * timestep;
	}
	if (Nigozi::Input::IsKeyPressed(GLFW_KEY_UP)) {
		m_luigiPosition.y += 2.0f * timestep;
	}
	if (Nigozi::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {
		glm::vec2 mousePosition = p_cameraLayer->GetCamera().GetMousePositionWorldSpace();
		m_particleSystem.Emit(Nigozi::ParticleProps{
			{ mousePosition.x, mousePosition.y },
			glm::vec2(0.4f, 0.4f), glm::vec2(0.1f, 0.1f),
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.2f, 0.0f, 0.7f),
			0.02f, 0.005f, 0.01f,
			1.2f
			});
	}
	m_mouseDelta = Nigozi::Input::GetMousePositionDelta();
	m_mousePosition = Nigozi::Input::GetMousePosition();
	m_particleSystem.OnUpdate(timestep);

	m_elapsedTime += timestep;
	if (m_elapsedTime > 0.2f) {
		m_playerTexture->SetSlot(m_slotX++, 0);
		if (m_slotX > 10)
			m_slotX = 0;
		m_elapsedTime = 0.0f;
	}
}

void SandboxLayer::OnRender()
{
	float increment = 1 / 200.0f;
	glm::vec4 color(increment, increment * increment, 1.0f, 1.0f);

	for (float i = 1 / 200.0f; i < 1; i += increment) {
		for (float j = 1 / 200.0f; j < 1; j += increment) {
			Nigozi::Renderer2D::DrawQuad({ i, j }, { increment, increment }, nullptr, color);
			color.z -= (increment * increment);
			color.y = i * j;
		}
		color.x += increment;
	}
	Nigozi::Renderer2D::DrawQuad(m_luigiPosition, { 0.2f, 0.2f }, m_luigiTexture, { 1.0f, 1.0f, 1.0f, 1.0f });
	Nigozi::Renderer2D::DrawQuad({ 1.0f, 1.0f }, { 0.2f, 0.2f }, m_playerTexture, glm::vec4(1));
	m_particleSystem.OnRender();
}

void SandboxLayer::OnImGuiRender()
{
	ImGui::Begin("Test Window!");

	glm::vec2 mousePosition = p_cameraLayer->GetCamera().GetMousePositionWorldSpace();

	ImGui::Text("Congratulations! Yo balls!");
	ImGui::Text("Mouse Delta");
	ImGui::Text((std::to_string(m_mouseDelta.first) + ", " + std::to_string(m_mouseDelta.second)).c_str());
	ImGui::Text("Mouse Position");
	ImGui::Text((std::to_string(m_mousePosition.first) + ", " + std::to_string(m_mousePosition.second)).c_str());
	ImGui::Text("Mouse Position World Space");
	ImGui::Text((std::to_string(mousePosition.x) + ", " + std::to_string(mousePosition.y)).c_str());
	ImGui::Text("Space key state");
	ImGui::Text(m_keystate.c_str());
	ImGui::Text("Draw Calls");
	ImGui::Text(std::to_string(Nigozi::Renderer2D::GetData()->DrawCalls).c_str());
	ImGui::Text("Quad Count");
	ImGui::Text(std::to_string(Nigozi::Renderer2D::GetData()->QuadCount).c_str());

	ImGui::Text("Audio Controls:");
	ImGui::Text("P -> Play");
	ImGui::Text("+ -> Volume Up");
	ImGui::Text("- -> Volume Down");
	ImGui::Text(("Volume: " + std::to_string(m_audioVolume)).c_str());

	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}

bool SandboxLayer::OnMouseButtonPressed(Nigozi::MouseButtonPressedEvent& event)
{
	if (event.GetButton() == GLFW_MOUSE_BUTTON_1) {
		NG_CLIENT_LOG_INFO("Wow, you managed to click a button");
		return true;
	}
	return false;
}

bool SandboxLayer::OnFullscreenToggle(Nigozi::KeyPressedEvent& event) {
	if (event.GetKeyCode() != GLFW_KEY_F11) {
		return false;
	}
	Nigozi::Window::SetGlobalFullscreen(!Nigozi::Window::IsGlobalFullscreen());
	return true;
}
