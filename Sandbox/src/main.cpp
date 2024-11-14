#include <Nigozi.h>
#include "Nigozi/core/EntryPoint.h"

#include "SandboxLayer.h"

namespace Global 
{
	static Nigozi::OrthographicCameraController cameraLayer = Nigozi::OrthographicCameraController(16 / 9.0f);
	static SandboxLayer sandboxLayer = SandboxLayer();
}

Nigozi::ApplicationProps CreateApplicationProps() {
	return { "Test App", 960, 540, true, false };
}

void OnApplicationInitialized(Nigozi::Application& app) {
	app.PushLayer(&Global::cameraLayer);
	app.PushLayer(&Global::sandboxLayer);
}
