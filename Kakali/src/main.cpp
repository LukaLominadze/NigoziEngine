#define MAIN

#include <Nigozi.h>
#include "KakaliApplication.h"

Nigozi::Application* CreateApplication() {
	KakaliApplication* app = new KakaliApplication({ "Kakali Editor", 960, 540, true, false });
	return app;
}