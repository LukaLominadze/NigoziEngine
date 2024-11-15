#pragma once

#include "Application.h"

extern Nigozi::ApplicationProps CreateApplicationProps();
extern void OnApplicationInitialized(Nigozi::Application& app);

// Define MAIN before including Nigozi.h in the main file of the program to set the entry point
#ifdef MAIN

#if NG_PLATFORM_WINDOWS
	#ifdef DISTRIBUTION
	#include <windows.h>
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
		Nigozi::Application app(CreateApplicationProps());
		OnApplicationInitialized(app);
		app.Run();
		return 0;
	}
	#else
	int main() {
		Nigozi::Application app(CreateApplicationProps());
		OnApplicationInitialized(app);
		app.Run();
	}
	#endif
#endif

#endif