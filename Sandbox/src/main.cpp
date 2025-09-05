#define MAIN

#include <Nigozi.h>
#include "Sandbox.h"

Nigozi::Application* CreateApplication() {
	Nigozi::ApplicationProps props;
	props.Title = "Test App";
	props.VSync = true;
	Nigozi::Application* app = new Sandbox(props);
	return app;
}
