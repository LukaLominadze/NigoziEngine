#pragma once
#include <Nigozi.h>
#include "SandboxLayer.h"

namespace Global
{
	inline Nigozi::OrthographicCameraController cameraLayer(16 / 9.0f);
	inline SandboxLayer sandboxLayer;
}