#define MAIN

#include <Nigozi.h>
#include "SandboxLayer.h"
#include "Global.h"

Nigozi::Application* CreateApplication() {
	Nigozi::Application* app = new Nigozi::Application({ "Test App", 960, 540, true, false });
	app->PushLayer(&Global::cameraLayer);
	app->PushLayer(&Global::sandboxLayer);
	return app;
}
