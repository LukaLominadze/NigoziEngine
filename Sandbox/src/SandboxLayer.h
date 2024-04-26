#pragma once
#include <Nigozi.h>

using namespace Nigozi;

class SandboxLayer : public Layer {
public:
	SandboxLayer();
	~SandboxLayer() = default;

	virtual void OnUpdate() override;
private:
	GameObject m_object = GameObject();
};
