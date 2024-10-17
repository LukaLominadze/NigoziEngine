#include <Nigozi.h>
#include "Nigozi/core/EntryPoint.h"

#include "SandboxLayer.h"

namespace Global 
{
	static SandboxLayer sandboxLayer = SandboxLayer();
}

Nigozi::Application* CreateApplication() {
	Nigozi::Application* app = new Nigozi::Application("Test", 960, 540, true, false);
	app->PushLayer(&Global::sandboxLayer);
	return app;
}
