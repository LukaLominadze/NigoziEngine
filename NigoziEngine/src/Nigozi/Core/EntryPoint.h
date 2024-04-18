#pragma once

#include "Nigozi/Core/Application.h"

namespace Nigozi {

	extern Application* CreateApplication();
}

int main(int argc, char** argv) {
	auto app = Nigozi::CreateApplication();
	app->Run();
	delete app;
	return 0;
}