#pragma once

#include "Window.h"

#include <unordered_map>

namespace Nigozi
{
	class Input {
	public:
		Input();
		~Input();

		static bool IsKeyJustPressed(int keycode);
		static bool IsKeyPressed(int keycode);
		static bool IsKeyReleased(int keycode);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		inline static std::pair<float, float> GetMousePositionDelta() { return s_instance->GetMousePosDelta(); }
		static float GetMouseX();
		static float GetMouseY();

		void OnUpdate();

		std::unordered_map<int, uint16_t> m_keystates;
		inline const std::pair<float, float>& GetMousePosDelta() { return m_mouseDeltaPos; }
	private:
		static Input* s_instance;


		std::pair<float, float> m_mouseStartPos;
		std::pair<float, float> m_mouseEndPos;
		std::pair<float, float> m_mouseDeltaPos;
	};
}