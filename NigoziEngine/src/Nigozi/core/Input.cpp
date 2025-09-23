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
        }
    }
}
