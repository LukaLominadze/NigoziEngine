#pragma once

#include "Application.h"

extern Nigozi::Application* CreateApplication();

#if NG_PLATFORM_WINDOWS
	#ifdef DISTRIBUTION
	#include <windows.h>
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
		Nigozi::Application* app = CreateApplication();
		app->Run();
		delete app;
		return 0;
	}
	#else
	int main() {
		Nigozi::Application* app = CreateApplication();
		app->Run();
		delete app;
	}
	#endif
#endif