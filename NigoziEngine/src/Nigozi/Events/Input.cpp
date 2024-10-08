#include "ngpch.h"
#include "Input.h"

namespace Nigozi
{
	NG_API Input* Input::s_instance = nullptr;

	Input::Input(EventHandler* eventHandler)
	{
		if (!s_instance) {
			s_instance = this;
			p_eventHandler = eventHandler;
		}
	}

	Input::~Input()
	{
		p_eventHandler = nullptr;
	}
}
