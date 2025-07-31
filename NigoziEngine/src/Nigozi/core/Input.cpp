#include "ngpch.h"

#include "Input.h"

namespace Nigozi
{
    namespace Input 
    {
        bool IsKeyPressed(int keycode)
        {
            int state = glfwGetKey(glfwGetCurrentContext(), keycode);
            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }

        bool IsKeyReleased(int keycode)
        {
            int state = glfwGetKey(glfwGetCurrentContext(), keycode);
            return state == GLFW_RELEASE;
        }

        float GetAxis(int negativeKeycode, int positiveKeycode)
        {
            return -(static_cast<float>(IsKeyPressed(negativeKeycode))) +
                    (static_cast<float>(IsKeyPressed(positiveKeycode)));
        }

        bool IsMouseButtonPressed(int button)
        {
            int state = glfwGetMouseButton(glfwGetCurrentContext(), button);
            return state == GLFW_PRESS;
        }

        std::pair<float, float> GetMousePosition()
        {
            double xpos, ypos;
            glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
            return std::pair<float, float>((float)xpos, (float)ypos);
        }

        float GetMouseX()
        {
            return GetMousePosition().first;
        }

        float GetMouseY()
        {
            return GetMousePosition().second;
        }

        void OnUpdate()
        {
            std::pair<float, float> endPos = GetMousePosition();
            MousePosDelta = std::pair<float, float>(endPos.first - MouseStartPos.first,
                                                    endPos.second - MouseStartPos.second);
            MouseStartPos = endPos;
        }
    }
}
