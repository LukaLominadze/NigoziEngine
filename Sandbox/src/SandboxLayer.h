#pragma once
#include <Nigozi.h>

using namespace Nigozi;

class SandboxLayer : public Layer {
public:
	SandboxLayer();
	~SandboxLayer() = default;
private:
	GameObject m_object = GameObject();
};
