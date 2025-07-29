#include "ngpch.h"

#include "Application.h"
#include "glcore/Renderer2D.h"

namespace Nigozi
{
    Application::Application(const ApplicationProps& props)
    {
        if (!CreateWindow(props)) {
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

        m_initialized = true;
        LOG("\nWelcome To NigoziEngine!");
    }

    Application::~Application()
    {
        Renderer2D::Deinitialize();
        delete p_window;
    }

    void Application::Run()
    {
        Test::Timer timer = Test::Timer();
        float timestep = 0.0f;
        while (!p_window->ShouldClose())
        {
            timer.StartTimerAndReturnSeconds();

            p_window->PollEvents();

            OnEvent();
            OnUpdate(timestep);
            OnRender();
            OnImGuiRender();

            p_window->Update();

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

    void Application::QueueEvent(std::function<Event* ()>&& func)
    {
        m_eventQueue.push(func);
    }

    void Application::OnEvent()
    {
        /*
            The events are created on the stack with _malloca(), 
            we do this so we can have different types of events
            (MousePressed, KeyPressed, etc.) that have different
            amount of data and size (12 bytes vs 16 bytes, etc.)
            and also have them stack allocated.
            To ensure these are created on the stack and that
            they can be used as different types of events,
            we use _malloca, then configure the event and then return
            as Event*
            To see how these functions are created, go to Window::CreateCallbacks()
        */
        std::scoped_lock<std::mutex> lock(m_eventQueueMutex);
        while (m_eventQueue.size() > 0) {
            std::function<Event* ()>& func = m_eventQueue.front();
            Event* event = func();
            if (event->GetEventType() == EventType::WindowClose) {
                Window::Close();
            }
            for (auto it = m_layerStack.end(); it != m_layerStack.begin(); )
            {
                (*--it)->OnEvent(*event);
                if (event->m_Handled)
                    break;
            }
            _freea(event);
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
    bool Application::CreateWindow(const ApplicationProps& props)
    {
        p_window = new Window();
        if (!p_window->StartUp(props.Title, props.Width, props.Height, props.Fullscreen)) {
            return false;
        }
        // Polled events will be sent to the OnEvent function in the application
        // Because the application class handles the distribution of all occurred events
        p_window->SetEventCallback(std::bind(&Application::QueueEvent, this, std::placeholders::_1));
        p_window->SetVSync(props.VSync);

        if (props.IconPath) {
            p_window->SetIcon(props.IconPath);
        }

        return p_window;
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
