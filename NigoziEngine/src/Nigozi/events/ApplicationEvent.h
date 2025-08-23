#pragma once

#include "Event.h"

namespace Nigozi
{
	class WindowResizedEvent : public Event {
	public:
		inline void Initialize(int width, int height) { m_type = EventType::WindowResize; m_Handled = false; m_width = width; m_height = height; }

		inline const int GetWidth() const { return m_width; }
		inline const int GetHeight() const { return m_height; }

		inline static EventType GetStaticType() { return EventType::WindowResize; }
	private:
		int m_width, m_height;
	};

	class WindowClosedEvent : public Event {
	public:
		inline void Initialize() { m_type = EventType::WindowClose; m_Handled = false; }
	};
}