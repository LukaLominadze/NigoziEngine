#pragma once
#include <Nigozi.h>

using namespace Nigozi;

class SandboxLayer : public Layer {
public:
	SandboxLayer() = default;
	~SandboxLayer() = default;

	virtual void OnAttach() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
private:
	GameObject m_object1 = GameObject();
	GameObject m_object2 = GameObject();
};
