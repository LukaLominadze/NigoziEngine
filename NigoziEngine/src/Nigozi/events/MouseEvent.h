#pragma once

#include "Event.h"
namespace Nigozi
{
	class MouseButtonEvent : public Event {
	protected:
		MouseButtonEvent(EventType type, int button)
			:Event(type), m_button(button)
		{}
	public:
		inline int GetButton() const { return m_button; }
	protected:
		int m_button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button)
			:MouseButtonEvent(EventType::MouseButtonPressed, button)
		{}

		inline void Initialize(int button) { m_type = EventType::MouseButtonPressed; m_button = button; }

		inline static EventType GetStaticType() { return EventType::MouseButtonPressed; }
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button)
			:MouseButtonEvent(EventType::MouseButtonReleased, button)
		{}

		inline void Initialize(int button) { m_type = EventType::MouseButtonReleased; m_button = button; }

		inline static EventType GetStaticType() { return EventType::MouseButtonReleased; }
	};

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y)
			:Event(EventType::MouseMoved), m_x(x), m_y(y)
		{}

		inline void Initialize(float x, float y) { m_type = EventType::MouseMoved; m_x = x; m_y = y; }

		inline const float GetX() const { return m_x; }
		inline const float GetY() const { return m_y; }

		inline static EventType GetStaticType() { return EventType::MouseMoved; }
	private:
		float m_x, m_y;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float x, float y)
			:Event(EventType::MouseScrolled), m_offsetX(x), m_offsetY(y)
		{}

		inline void Initialize(float x, float y) { m_type = EventType::MouseScrolled; m_offsetX = x; m_offsetY = y; }

		inline const float GetOffsetX() const { return m_offsetX; }
		inline const float GetOffsetY() const { return m_offsetY; }

		inline static EventType GetStaticType() { return EventType::MouseScrolled; }
	private:
		float m_offsetX, m_offsetY;
	};
}