#pragma once

#include "Nigozi/Core/Core.h"

namespace Nigozi 
{
	enum class NG_API EventType {
		None = 0,
		WindowClose = SDL_QUIT, WindowResize = SDL_WINDOWEVENT,
		KeyPressed = SDL_KEYDOWN, KeyDown = 1, KeyReleased = SDL_KEYUP
	};
}
