#include "ngpch.h"

#include "Window.h"

#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace Nigozi
{
    Window::Window(const char* title, uint32_t width, uint32_t height, bool fullscreen = false)
    {
        int initialization = glfwInit();
        ASSERT(initialization, "Initializing GLFW...");

        Global::windowData.Width = width;
        Global::windowData.Height = height;
        Global::windowData.Fullscreen = fullscreen;

        p_window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!p_window)
        {
            ASSERT(p_window, "Initializing Window...");
            glfwTerminate();
        }

        Global::windowData.NativeWindow = p_window;

        // Get the monitor to set the viewport and fullscreen mode
        p_monitor = glfwGetPrimaryMonitor();

        if (Global::windowData.Fullscreen) {
            const GLFWvidmode* mode = glfwGetVideoMode(p_monitor);
            Global::windowData.Width = mode->width;
            Global::windowData.Height = mode->height;
            glfwSetWindowMonitor(p_window, p_monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
            glViewport(0, 0, mode->width, mode->height);
        }

        glfwMakeContextCurrent(p_window);

        if (glewInit() != GLEW_OK) {
            ASSERT(false, "Initializing GLEW...");
        }

        glfwSetWindowUserPointer(p_window, &Global::windowData);

        // Get and send the events to application
        glfwSetWindowCloseCallback(p_window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowClosedEvent event;
                data.EventCallback(event);
            });

        glfwSetWindowSizeCallback(p_window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;
                glViewport(0, 0, data.Width, data.Height);
                WindowResizedEvent event(width, height);
                data.EventCallback(event);
            });

        glfwSetKeyCallback(p_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                }
            });

        glfwSetMouseButtonCallback(p_window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                }
            });

        glfwSetScrollCallback(p_window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseScrolledEvent event((float)xOffset, (float)yOffset);
                data.EventCallback(event);
            });
    }

    Window::~Window()
    {
        if (p_window) {
            Delete();
        }
    }

    void Window::SetVSync(bool value)
    {
        if (value)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
    }

    void Window::OnEvent(Event& e)
    {
        EventDispatcher dispatcher = EventDispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(std::bind(&Window::OnFullscreenToggle, this, std::placeholders::_1));
    }

    void Window::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(p_window);
    }

    void Window::Delete()
    {
        glfwDestroyWindow(p_window);
    }

    bool Window::OnFullscreenToggle(KeyPressedEvent& e)
    {
        if (e.GetKeyCode() != GLFW_KEY_F11)
            return false;
        Global::windowData.Fullscreen = !Global::windowData.Fullscreen;
        const GLFWvidmode* mode = glfwGetVideoMode(p_monitor);
        if (Global::windowData.Fullscreen) {
            glfwSetWindowMonitor(p_window, p_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            glViewport(0, 0, mode->width, mode->height);
        }
        else {
            int xpos = (mode->width / Global::windowData.Width) / 2;
            int ypos = (mode->height / Global::windowData.Height) / 2 + 40;
            glfwSetWindowMonitor(p_window, NULL, xpos, ypos, Global::windowData.Width, Global::windowData.Height, 0);
        }
        return true;
    }
}
