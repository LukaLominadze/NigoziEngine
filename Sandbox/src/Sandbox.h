#pragma once
#include <Nigozi.h>
#include "SandboxLayer.h"

using namespace Nigozi;

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