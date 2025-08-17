#include "ngpch.h"

#include "Application.h"
#include "glcore/Renderer2D.h"

namespace Nigozi
{
    Application::Application(const ApplicationProps& props)
        :m_window(props.Title, props.Width, props.Height, props.Fullscreen, props.VSync)
    {
        if (!SetupWindow(props)) {
            std::cout << "Couldn't create window! Shutting down..." << std::endl;
            return;
        }

        if (!CreateGUILayer()) {
            std::cout << "Couldn't create GUI layer! Shutting down..." << std::endl;
            return;
        }

        if (!StartRenderer()) {
            std::cout << "Couldn't initialize renderer! Shutting down..." << std::endl;
            return;
        }

        p_eventBufferPointer = (Event*)(m_eventBuffer);

        m_initialized = true;
        LOG("\nWelcome To NigoziEngine!");
    }

    Application::~Application()
    {
        Renderer2D::Deinitialize();
    }

    void Application::Run()
    {
        Test::Timer timer = Test::Timer();
        float timestep = 0.0f;
        while (!m_window.ShouldClose())
        {
            timer.StartTimerAndReturnSeconds();

            m_window.PollEvents();

            OnEvent();
            OnUpdate(timestep);
            OnRender();
            OnImGuiRender();

            m_window.Update();

            timestep = timer.EndTimerAndReturnSeconds();
        }
    }

    void Application::Close()
    {
        Window::Close();
    }

    void Application::PushLayer(Layer* layer)
    {
        m_layerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_layerStack.PushOverlay(layer);
    }

    void Application::PopLayer(Layer* layer)
    {
        m_layerStack.PopLayer(layer);
    }

    void Application::PopOverlay(Layer* layer)
    {
        m_layerStack.PopOverlay(layer);
    }

    void Application::QueueEvent(std::function<void(Event*)>&& func)
    {
        m_eventQueue.push(func);
    }

    void Application::OnEvent()
    {
        /*
            The event pointer that is passed
            in the queued functions will initialize
            the buffer as the queued event
            for ex.: WindowResizeEvent, KeyEvent, etc.
        */
        std::scoped_lock<std::mutex> lock(m_eventQueueMutex);
        while (m_eventQueue.size() > 0) {
            std::function<void(Event*)>& func = m_eventQueue.front();
            func(p_eventBufferPointer);
            if (p_eventBufferPointer->GetEventType() == EventType::WindowClose) {
                Window::Close();
            }
            for (auto it = m_layerStack.end(); it != m_layerStack.begin(); )
            {
                (*--it)->OnEvent(*p_eventBufferPointer);
                if (p_eventBufferPointer->m_Handled)
                    break;
            }
            m_eventQueue.pop();
        }
    }

    void Application::OnUpdate(float timestep)
    {
        Input::OnUpdate();
        for (Layer* layer : m_layerStack) {
            layer->OnUpdate(timestep);
        }
    }

    void Application::OnRender()
    {
        Renderer2D::BeginScene();
        Renderer2D::Clear();
        for (Layer* layer : m_layerStack) {
            layer->OnRender();
        }
        Renderer2D::EndScene();
    }

    void Application::OnImGuiRender()
    {
        m_imGuiLayer.Begin();
        for (Layer* layer : m_layerStack) {
            layer->OnImGuiRender();
        }
        m_imGuiLayer.End();
    }

    // Window class is used for initializing the context,
    // handling GLFW functions (like vsync, fullscreen, etc.)
    // and events
    bool Application::SetupWindow(const ApplicationProps& props)
    {
        if (!m_window.Initialized()) {
            return false;
        }
        // Polled events will be sent to the OnEvent function in the application
        // Because the application class handles the distribution of all occurred events
        m_window.SetEventCallback(std::bind(&Application::QueueEvent, this, std::placeholders::_1));
        m_window.SetVSync(props.VSync);

        if (props.IconPath) {
            m_window.SetIcon(props.IconPath);
        }

        return true;
    }

    bool Application::CreateGUILayer()
    {
        // ImGUI is an overlay, because we want to render UI last and for UI to
        // get the events FIRST
        PushOverlay(&m_imGuiLayer);
        return &m_imGuiLayer;
    }

    bool Application::StartRenderer()
    {
        Renderer2D::Initialize();
        Renderer2D::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        return Renderer2D::GetData()->QuadVertexArray;
    }
}
