#include "ngpch.h"

#include "Input.h"
#include "core/Log.h"

namespace Nigozi
{
    namespace Input 
    {
        static std::unordered_map<int, int> s_keyStates;
        static std::unordered_map<int, int> s_queuedStates;

        bool IsKeyJustPressed(int keycode)
        {
            int state = glfwGetKey(glfwGetCurrentContext(), keycode);
            if (state == GLFW_PRESS && s_keyStates[keycode] != GLFW_PRESS) {
                s_queuedStates[keycode] = GLFW_PRESS;
                return true;
            }
            return false;
        }

        bool IsKeyPressed(int keycode)
        {
            int state = glfwGetKey(glfwGetCurrentContext(), keycode);
            s_queuedStates[keycode] = state;
            return state == GLFW_PRESS;
        }

        bool IsKeyJustReleased(int keycode)
        {
            int state = glfwGetKey(glfwGetCurrentContext(), keycode);
            if (state == GLFW_RELEASE && s_keyStates[keycode] != GLFW_RELEASE) {
                s_queuedStates[keycode] = GLFW_RELEASE;
                return true;
            }
            return false;
        }

        bool IsKeyReleased(int keycode)
        {
            int state = glfwGetKey(glfwGetCurrentContext(), keycode);
            s_queuedStates[keycode] = state;
            return state == GLFW_RELEASE;
        }

        float GetAxis(int negativeKeycode, int positiveKeycode)
        {
            return -(static_cast<float>(IsKeyPressed(negativeKeycode))) +
                    (static_cast<float>(IsKeyPressed(positiveKeycode)));
        }

        glm::vec2 GetAxis(glm::i32vec2 negativeXKeycodes, glm::i32vec2 positiveYKeycodes)
        {
            return glm::vec2(
                GetAxis(negativeXKeycodes.x, negativeXKeycodes.y),
                GetAxis(positiveYKeycodes.x, positiveYKeycodes.y)
            );
        }

        bool IsMouseButtonJustPressed(int button)
        {
            int state = glfwGetMouseButton(glfwGetCurrentContext(), button);
            if (state == GLFW_PRESS && s_keyStates[button] != GLFW_PRESS) {
                s_keyStates[button] = GLFW_PRESS;
                return true;
            }
            return false;
        }

        bool IsMouseButtonPressed(int button)
        {
            int state = glfwGetMouseButton(glfwGetCurrentContext(), button);
            return state == GLFW_PRESS;
        }

        bool IsMouseButtonJustReleased(int button)
        {
            int state = glfwGetMouseButton(glfwGetCurrentContext(), button);
            if (state == GLFW_RELEASE && s_keyStates[button] != GLFW_RELEASE) {
                s_keyStates[button] = GLFW_RELEASE;
                return true;
            }
            return false;
        }

        bool IsMouseButtonReleased(int button)
        {
            int state = glfwGetMouseButton(glfwGetCurrentContext(), button);
            return state == GLFW_RELEASE;
        }

        glm::vec2 GetMousePosition()
        {
            double xpos, ypos;
            glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
            return glm::vec2((float)xpos, (float)ypos);
        }

        float GetMouseX()
        {
            return GetMousePosition().x;
        }

        float GetMouseY()
        {
            return GetMousePosition().y;
        }

        void OnUpdate()
        {
            glm::vec2 endPos = GetMousePosition();
            MousePosDelta = glm::vec2(endPos.x - MouseStartPos.x,
                                      endPos.y - MouseStartPos.y);
            MouseStartPos = endPos;

            for (auto [key, value] : s_queuedStates) {
                IsKeyReleased(key);
                s_keyStates[key] = value;
            }
        }
    }
}
