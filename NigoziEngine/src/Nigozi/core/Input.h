#pragma once

#include "Window.h"

#include "ngpch.h"

namespace Nigozi
{
	namespace Input 
	{
		inline glm::vec2 MouseStartPos = { 0, 0 };
		inline glm::vec2 MousePosDelta = { 0, 0 };

		bool IsKeyPressed(int keycode);
		bool IsKeyReleased(int keycode);
		float GetAxis(int negativeKeycode, int positiveKeycode);
		bool IsMouseButtonPressed(int button);
		glm::vec2 GetMousePosition();
		inline glm::vec2 GetMousePositionDelta() { return MousePosDelta; }
		float GetMouseX();
		float GetMouseY();
		void OnUpdate();
	}
}