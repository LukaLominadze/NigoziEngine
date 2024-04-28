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

    bool EventHandler::GetWindowEvent(SDL_EventType type)
    {
        if (m_event.type == type) {
            return true;
        }
        return false;
    }

    bool EventHandler::KeyPressed(SDL_Scancode keycode)
    {
        if (m_event.type != SDL_KEYDOWN) return false;
        if (m_event.key.keysym.scancode == keycode && m_lastkey != keycode) {
            LOG("KeyPressed! " << keycode);
            m_lastkey = keycode;
            return true;
        }
        return false;
    }

    bool EventHandler::KeyDown(SDL_Scancode keycode)
    {
        if (keystate[keycode]) {
            LOG("KeyDown! " << keycode);
            return true;
        }
        return false;
    }

    bool EventHandler::KeyReleased(SDL_Scancode keycode)
    {
        if (m_event.type != SDL_KEYUP) return false;
        if (m_event.key.keysym.scancode == keycode) {
            LOG("KeyRekeased! " << keycode);
            return true;
        }
        return false;
    }
}
