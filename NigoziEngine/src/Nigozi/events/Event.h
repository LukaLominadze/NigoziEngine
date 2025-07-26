#pragma once

namespace Nigozi
{
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	class Event {
	public:
		Event(EventType type)
			:m_type(type)
		{}

		bool Handled = false;

		inline EventType GetEventType() const { return m_type; }

	protected:
		EventType m_type;
	};

	class EventDispatcher {
	public:
		EventDispatcher(Event& event)
			:m_event(event)
		{
		}

		template<typename T, typename F>
		void Dispatch(const F& function) {
			if (m_event.GetEventType() == T::GetStaticType()) {
				m_event.Handled = function(static_cast<T&>(m_event));
			}
		}

	private:
		Event& m_event;
	};
}
