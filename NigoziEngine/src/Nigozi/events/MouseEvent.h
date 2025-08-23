#pragma once

#include "Event.h"
namespace Nigozi
{
	class MouseButtonEvent : public Event {
	public:
		inline int GetButton() const { return m_button; }
	protected:
		int m_button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		inline void Initialize(int button) { m_type = EventType::MouseButtonPressed; m_Handled = false; m_button = button; }

		inline static EventType GetStaticType() { return EventType::MouseButtonPressed; }
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		inline void Initialize(int button) { m_type = EventType::MouseButtonReleased; m_Handled = false; m_button = button; }

		inline static EventType GetStaticType() { return EventType::MouseButtonReleased; }
	};

	class MouseMovedEvent : public Event {
	public:
		inline void Initialize(float x, float y) { m_type = EventType::MouseMoved; m_Handled = false; m_x = x; m_y = y; }

		inline const float GetX() const { return m_x; }
		inline const float GetY() const { return m_y; }

		inline static EventType GetStaticType() { return EventType::MouseMoved; }
	private:
		float m_x, m_y;
	};

	class MouseScrolledEvent : public Event {
	public:
		inline void Initialize(float x, float y) { m_type = EventType::MouseScrolled; m_Handled = false; m_offsetX = x; m_offsetY = y; }

		inline const float GetOffsetX() const { return m_offsetX; }
		inline const float GetOffsetY() const { return m_offsetY; }

		inline static EventType GetStaticType() { return EventType::MouseScrolled; }
	private:
		float m_offsetX, m_offsetY;
	};
}