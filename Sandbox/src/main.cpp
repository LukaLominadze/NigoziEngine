#include "Nigozi.h"

using namespace Nigozi;

class Sandbox : public Application {
public:
	Sandbox() : Application("MyGame", 1366, 160) {
		m_object.AddComponent<Sprite>("Player.png");
	}
	
	void OnStart() override {
		
	}

	void OnUpdate() override {
		EventHandler::Get().GetKey(EventType::KeyPressed, SDL_SCANCODE_LEFT);
		EventHandler::Get().GetKey(EventType::KeyReleased, SDL_SCANCODE_SPACE);
		EventHandler::Get().GetKey(EventType::KeyDown, SDL_SCANCODE_A);
	}

	void OnRender() override {
		SDL_Rect m_rect = { 100, 100, 50, 50 };
		NG_Renderer->DrawRect(m_rect, 20, 60, 23, 255);	

		SDL_Delay(1000 / 60);
	}
	
	~Sandbox() {

	}
private:
	GameObject m_object = GameObject();
};

Application* Nigozi::CreateApplication() {
	return new Sandbox();
}
