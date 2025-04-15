#pragma once

#include "Application.h"

// Define MAIN before including Nigozi.h in the main file of the program to set the entry point
#ifdef MAIN

extern Nigozi::Application* CreateApplication();
//extern Nigozi::ApplicationProps CreateApplicationProps();
//extern void OnApplicationInitialized(Nigozi::Application& app);

#if DISTRIBUTION
	#ifdef NG_PLATFORM_WINDOWS
		#include <windows.h>
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
			Nigozi::Application* app = CreateApplication();
			app->Run();
			delete app;
			return 0;
		}
	#elif NG_PLATFORM_LINUX
		#error Linux not supported on DISTRIBUTION yet!
	#else
		#error NigoziEngine DISTRIBUTION configuration is not supported on this platform!
	#endif
#else
int main(int argc, char* argv[]) {
	Nigozi::Application* app = CreateApplication();
	app->Run();
	delete app;
}
#endif
#endif