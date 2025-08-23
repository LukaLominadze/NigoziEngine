#pragma once

#include "Application.h"
#include "Log.h"

// Define MAIN before including Nigozi.h in the main file of the program to set the entry point
#ifdef MAIN

extern Nigozi::Application* CreateApplication();

#ifdef DISTRIBUTION and NG_PLATFORM_WINDOWS
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Nigozi::Log::Initialize();
	Nigozi::Application* app = CreateApplication();
	if (app->Initialized()) {
		app->Run();
	}
	delete app;
	return 0;
}
#else
int main(int argc, char* argv[]) {
	Nigozi::Log::Initialize();
	Nigozi::Application* app = CreateApplication();
	if (app->Initialized()) {
		app->Run();
	}
	delete app;
}
#endif
#endif