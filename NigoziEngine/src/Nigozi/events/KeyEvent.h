#pragma once

#include "Event.h"

namespace Nigozi
{
	class KeyEvent : public Event {
	public:
		const int GetKeyCode() const { return m_keyCode; }
	protected:
		KeyEvent(EventType type, int keyCode)
			:Event(type), m_keyCode(keyCode)
		{}
	protected:
		int m_keyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keyCode)
			:KeyEvent(EventType::KeyPressed, keyCode)
		{}

		inline void Initialize(int keyCode) { m_type = EventType::KeyPressed; m_keyCode = keyCode; }

		inline static EventType GetStaticType() { return EventType::KeyPressed; }
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keyCode)
			:KeyEvent(EventType::KeyReleased, keyCode)
		{}

		inline void Initialize(int keyCode) { m_type = EventType::KeyReleased; m_keyCode = keyCode; }

		inline static EventType GetStaticType() { return EventType::KeyReleased; }
	};

	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keyCode)
			:KeyEvent(EventType::KeyTyped, keyCode)
		{}

		inline void Initialize(int keyCode) { m_type = EventType::KeyTyped; m_keyCode = keyCode; }

		inline static EventType GetStaticType() { return EventType::KeyTyped; }
	};
}
