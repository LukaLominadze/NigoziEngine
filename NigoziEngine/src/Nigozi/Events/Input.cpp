#include "ngpch.h"
#include "Input.h"

namespace Nigozi
{
	Input* Input::s_instance = nullptr;

	Input::Input(EventHandler* eventHandler)
	{
		if (!s_instance) {
			s_instance = this;
			s_instance->p_eventHandler = eventHandler;
		}
	}

	Input::~Input()
	{
		p_eventHandler = nullptr;
	}
}
