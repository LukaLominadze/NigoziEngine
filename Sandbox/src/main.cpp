#include "Nigozi.h"

using namespace Nigozi;

class SandboxLayer : public Layer {
public:
	SandboxLayer() {
		m_object.AddComponent<Sprite>("Player.png");
		Sprite* p_sprite = m_object.GetComponent<Sprite>();
		p_sprite->data.srcrect = { 0, 0, 16, 16 };
		p_sprite->data.dstrect = { 150, 150, 300, 300 };
		p_sprite = nullptr;
	}
	~SandboxLayer() = default;
private:
	GameObject m_object = GameObject();
};

class Sandbox : public Application {
public:
	Sandbox() : Application("MyGame", 600, 600) {
		p_sandboxLayer = new SandboxLayer();
		PushLayer(p_sandboxLayer);
	}
	
	~Sandbox() = default;
private:
	SandboxLayer* p_sandboxLayer = nullptr;
};

Application* Nigozi::CreateApplication() {
	return new Sandbox();
}
