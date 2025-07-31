#define MAIN

#include <Nigozi.h>
#include "Sandbox.h"

Nigozi::Application* CreateApplication() {
	Nigozi::Application* app = new Sandbox({ "Test App", 960, 540, true, false });
	return app;
}
