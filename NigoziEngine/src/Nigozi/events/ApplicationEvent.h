#pragma once

#include "Event.h"

namespace Nigozi
{
	class WindowResizedEvent : public Event {
	public:
		WindowResizedEvent(int width, int height)
			:Event(EventType::WindowResize), m_width(width), m_height(height)
		{}

		inline const int GetWidth() const { return m_width; }
		inline const int GetHeight() const { return m_height; }

		inline static EventType GetStaticType() { return EventType::WindowResize; }
	private:
		int m_width, m_height;
	};

	class WindowClosedEvent : public Event {
	public:
		WindowClosedEvent()
			:Event(EventType::WindowClose)
		{}
	};
}