#pragma once

#include "Window.h"

#include "ngpch.h"

namespace Nigozi
{
	namespace Input 
	{
		inline glm::vec2 MouseStartPos = { 0, 0 };
		inline glm::vec2 MousePosDelta = { 0, 0 };

		bool IsKeyJustPressed(int keycode);
		bool IsKeyPressed(int keycode);
		bool IsKeyJustReleased(int keycode);
		bool IsKeyReleased(int keycode);

		float GetAxis(int negativeKeycode, int positiveKeycode);
		glm::vec2 GetAxis(glm::i32vec2 negativeXKeycodes, glm::i32vec2 positiveYKeycodes);

		bool IsMouseButtonJustPressed(int button);
		bool IsMouseButtonPressed(int button);
		bool IsMouseButtonJustReleased(int button);
		bool IsMouseButtonReleased(int button);

		glm::vec2 GetMousePosition();
		inline glm::vec2 GetMousePositionDelta() { return MousePosDelta; }
		
		float GetMouseX();
		float GetMouseY();
		void OnUpdate();
	}
}