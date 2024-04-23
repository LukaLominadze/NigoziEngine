#include "Nigozi.h"

using namespace Nigozi;

class Sandbox : public Application {
public:
	Sandbox() : Application("MyGame", 600, 600) {
		m_object.AddComponent<Sprite>("Player.png");
		Sprite* p_sprite = m_object.GetComponent<Sprite>();
		p_sprite->data.srcrect = { 0, 0, 16, 16 };
		p_sprite->data.dstrect = { 150, 150, 300, 300 };
		p_sprite = nullptr;
	}
	
	void OnStart() override {
		
	}

	void OnUpdate() override {
		EventHandler::Get().GetKey(EventType::KeyPressed, SDL_SCANCODE_LEFT);
		EventHandler::Get().GetKey(EventType::KeyReleased, SDL_SCANCODE_SPACE);
		EventHandler::Get().GetKey(EventType::KeyDown, SDL_SCANCODE_A);
	}

	void OnRender() override {
		SDL_Delay(1000 / 60);
	}
	
	~Sandbox() = default;
private:
	GameObject m_object = GameObject();
};

Application* Nigozi::CreateApplication() {
	return new Sandbox();
}
