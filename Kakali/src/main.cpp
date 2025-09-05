#define MAIN

#include <Nigozi.h>
#include "KakaliApplication.h"

Nigozi::Application* CreateApplication() {
	Nigozi::ApplicationProps props;
	props.Title = "Kakali Editor";
	props.Width = 1600;
	props.Height = 900;
	props.VSync = true;
	props.Fullscreen = false;
	props.IconPath = "kakali-small.png";
	KakaliApplication* app = new KakaliApplication(props);
	return app;
}