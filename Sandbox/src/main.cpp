#include <Nigozi.h>
#include "Sandbox.h"

using namespace Nigozi;

int main(int argc, char** argv) {
	auto app = Nigozi::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
