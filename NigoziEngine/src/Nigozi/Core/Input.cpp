#include "ngpch.h"

#include "Input.h"

namespace Nigozi
{
    Input* Input::s_instance = nullptr;

    Input::Input()
        :m_mouseStartPos(std::pair<float, float>()), m_mouseEndPos(std::pair<float, float>())
    {
        if (!s_instance) {
            s_instance = this;
        }
        // add all keycodes in keystates
        for (int i = 32; i < 349; i++) 
        {
            m_keystates[i] = GLFW_RELEASE;
        }
    }

    Input::~Input()
    {
        s_instance = nullptr;
    }

    bool Input::IsKeyJustPressed(int keycode)
    {
        int state = glfwGetKey(Global::windowData.NativeWindow, keycode);
        if (state == GLFW_PRESS) {
            if (s_instance->m_keystates[keycode] == GLFW_RELEASE) {
                s_instance->m_keystates[keycode] = GLFW_PRESS;
                return true;
            }
        }
        return false;
    }

    bool Input::IsKeyPressed(int keycode)
    {
        int state = glfwGetKey(Global::windowData.NativeWindow, keycode);
        if (state == GLFW_PRESS || state == GLFW_REPEAT) {
            if (s_instance->m_keystates[keycode] == GLFW_PRESS || s_instance->m_keystates[keycode] == GLFW_REPEAT)
            {
                s_instance->m_keystates[keycode] = GLFW_REPEAT;
                return true;
            }
        }
        return false;
    }

    bool Input::IsKeyReleased(int keycode)
    {
        int state = glfwGetKey(Global::windowData.NativeWindow, keycode);
        if (state == GLFW_RELEASE) {
            if (!s_instance->m_keystates[keycode] == state) {
                s_instance->m_keystates[keycode] = GLFW_RELEASE;
                return true;
            }
        }
        return false;
    }

    bool Input::IsMouseButtonPressed(int button)
    {
        int state = glfwGetMouseButton(Global::windowData.NativeWindow, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::GetMousePosition()
    {
        double xpos, ypos;
        glfwGetCursorPos(Global::windowData.NativeWindow, &xpos, &ypos);
        return std::pair<float, float>((float)xpos, (float)ypos);
    }

    float Input::GetMouseX()
    {
        return GetMousePosition().first;
    }

    float Input::GetMouseY()
    {
        return GetMousePosition().second;
    }

    void Input::OnUpdate()
    {
        m_mouseEndPos = GetMousePosition();
        m_mouseDeltaPos = std::pair<float, float>(m_mouseEndPos.first - m_mouseStartPos.first,
                                                  m_mouseEndPos.second - m_mouseStartPos.second);
        m_mouseStartPos = m_mouseEndPos;
    }
}
