#define MAIN

#include <Nigozi.h>
#include "KakaliApplication.h"

Nigozi::Application* CreateApplication() {
	KakaliApplication* app = new KakaliApplication({ "Kakali Editor", 1600, 900, true, false, "logo.png"});
	return app;
}