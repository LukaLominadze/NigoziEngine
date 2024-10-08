#include <Nigozi.h>
#include "Sandbox.h"

Application* Nigozi::CreateApplication() {
	return new Sandbox();
}
