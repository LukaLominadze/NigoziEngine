#include "ngpch.h"

#include "Window.h"

#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace Nigozi
{
    Window::~Window()
    {
        if (p_window) {
            Delete();
        }
        glfwTerminate();
    }

    bool Window::StartUp(const char* title, uint32_t width, uint32_t height, bool fullscreen)
    {
        if (!StartGLFW()) {
            std::cout << "Couldn't initialize GLFW...";
            return false;
        }

        if (!CreateWindow(title, width, height, fullscreen)) {
            std::cout << "Couldn't create GLFW window...";
            return false;
        }

        if (!StartGLEW()) {
            std::cout << "Couldn't initialize GLEW...";
            return false;
        }

        CreateCallbacks();
        return true;
    }

    bool Window::IsFullscreen()
    {
        return Global::windowData.Fullscreen;
    }

    void Window::SetVSync(bool value)
    {
        if (value)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
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

    bool Window::StartGLFW()
    {
        return glfwInit() == GLFW_TRUE;
    }

    bool Window::CreateWindow(const char* title, uint32_t width, uint32_t height, bool fullscreen)
    {
        Global::windowData.Width = width;
        Global::windowData.Height = height;
        Global::windowData.Fullscreen = fullscreen;

        p_window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!p_window)
        {
            return p_window;
        }

        glfwMakeContextCurrent(p_window);
        glfwSetWindowUserPointer(p_window, &Global::windowData);

        // Get the monitor to set the viewport and fullscreen mode
        GLCall(p_monitor = glfwGetPrimaryMonitor());

        if (Global::windowData.Fullscreen) {
            const GLFWvidmode* mode = glfwGetVideoMode(p_monitor);
            Global::windowData.Width = mode->width;
            Global::windowData.Height = mode->height;
            glfwSetWindowMonitor(p_window, p_monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
            GLCall(glViewport(0, 0, mode->width, mode->height));
        }
        return p_window;
    }

    bool Window::StartGLEW()
    {
        return glewInit() == GLEW_OK;
    }

    bool Window::CreateCallbacks()
    {
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
                GLCall(glViewport(0, 0, data.Width, data.Height));
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
        return true;
    }

    bool Window::SetFullscreen(bool value)
    {
        Global::windowData.Fullscreen = value;
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        GLFWwindow* window = glfwGetCurrentContext();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (Global::windowData.Fullscreen) {
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        else {
            int xpos = (mode->width / Global::windowData.Width) / 2;
            int ypos = (mode->height / Global::windowData.Height) / 2 + 40;
            glfwSetWindowMonitor(window, NULL, xpos, ypos, Global::windowData.Width, Global::windowData.Height, 0);
        }
        return true;
    }
}
