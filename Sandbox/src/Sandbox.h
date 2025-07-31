#pragma once
#include <Nigozi.h>
#include "SandboxLayer.h"

class Sandbox : public Nigozi::Application {
public:
	Sandbox(const Nigozi::ApplicationProps props);
private:
	Nigozi::OrthographicCameraController m_cameraLayer;
	SandboxLayer m_sandboxLayer;
};

