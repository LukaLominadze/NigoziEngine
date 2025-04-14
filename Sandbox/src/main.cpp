#define MAIN

#include <Nigozi.h>
#include "SandboxLayer.h"
#include "Global.h"

Nigozi::ApplicationProps CreateApplicationProps() {
	return { "Test App", 960, 540, true, false };
}

void OnApplicationInitialized(Nigozi::Application& app) {
	app.PushLayer(&Global::cameraLayer);
	app.PushLayer(&Global::sandboxLayer);
}
