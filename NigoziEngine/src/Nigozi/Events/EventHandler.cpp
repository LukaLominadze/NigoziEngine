#include "ngpch.h"
#include "EventHandler.h"

namespace Nigozi
{
    EventHandler* EventHandler::s_Instance = nullptr;

    EventHandler::EventHandler()
    {
        if (!s_Instance) {
            s_Instance = this;
        }
    }

    EventHandler::~EventHandler()
    {
    }

    void EventHandler::PollEvents()
    {
        SDL_PollEvent(&m_event);

        if (m_event.type == SDL_KEYUP) {
            m_lastkey = SDL_SCANCODE_UNKNOWN;
        }
    }

    bool EventHandler::GetWindowEvent(EventType type)
    {
        if (m_event.type == (SDL_EventType)type) {
            return true;
        }
        return false;
    }

    bool EventHandler::GetKey(EventType type, SDL_Scancode keycode)
    {
        switch (type) {
        case EventType::KeyDown:
            if (keystate[keycode]) {
                LOG("KeyDown! " << keycode);
                return true;
            }
            break;
        case EventType::KeyPressed:
            if (m_event.type != SDL_KEYDOWN) return false;
            if (m_event.key.keysym.scancode == keycode && m_lastkey != keycode) {
                LOG("KeyEvent! " << (SDL_EventType)type << " " << keycode);
                m_lastkey = keycode;
                return true;
            }
            break;
        case EventType::KeyReleased:
            if (m_event.type != SDL_KEYUP) return false;
            if (m_event.key.keysym.scancode == keycode) {
                LOG("KeyEvent! " << (SDL_EventType)type << " " << keycode);
                return true;
            }
            break;
        }
        return false;
    }
}
