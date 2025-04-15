#pragma once

#include <Nigozi.h>

class EditorLayer : public Nigozi::Layer
{
public:
	void OnImGuiRender() override;
};

