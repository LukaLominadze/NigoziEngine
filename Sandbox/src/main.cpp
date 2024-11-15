#define MAIN

#include <Nigozi.h>
#include "SandboxLayer.h"

namespace Global 
{
	static Nigozi::OrthographicCameraController cameraLayer(16 / 9.0f);
	static SandboxLayer sandboxLayer;
}

Nigozi::ApplicationProps CreateApplicationProps() {
	return { "Test App", 960, 540, true, false };
}

void OnApplicationInitialized(Nigozi::Application& app) {
	app.PushLayer(&Global::cameraLayer);
	app.PushLayer(&Global::sandboxLayer);
}
