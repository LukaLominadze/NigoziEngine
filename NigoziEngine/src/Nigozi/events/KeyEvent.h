#pragma once

#include "Event.h"

namespace Nigozi
{
	class KeyEvent : public Event {
	public:
		const int GetKeyCode() const { return m_keyCode; }
	protected:
		int m_keyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		inline void Initialize(int keyCode) { m_type = EventType::KeyPressed; m_Handled = false; m_keyCode = keyCode; }

		inline static EventType GetStaticType() { return EventType::KeyPressed; }
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		inline void Initialize(int keyCode) { m_type = EventType::KeyReleased; m_Handled = false; m_keyCode = keyCode; }

		inline static EventType GetStaticType() { return EventType::KeyReleased; }
	};

	class KeyTypedEvent : public KeyEvent {
	public:
		inline void Initialize(int keyCode) { m_type = EventType::KeyTyped; m_Handled = false; m_keyCode = keyCode; }

		inline static EventType GetStaticType() { return EventType::KeyTyped; }
	};
}
