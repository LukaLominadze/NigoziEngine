#include "ngpch.h"

#include "Application.h"

namespace Nigozi
{
    Application::Application(const ApplicationProps& props)
        :m_running(false), m_input(Input())
    {
        p_window = new Window(props.Title, props.Width, props.Height, props.Fullscreen);
        p_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        p_window->SetVSync(props.VSync);

        m_renderer = Renderer();

        PushOverlay(&m_imGuiLayer);
    }

    Application::~Application()
    {
        //delete p_input;
        p_window->Delete();

        glfwTerminate();
    }

    void Application::Run()
    {
        Test::Timer timer = Test::Timer();
        float timestep = 0.0f;

        m_running = true;

        while (m_running)
        {
            timer.StartTimerAndReturnSeconds();

            OnUpdate(timestep);
            OnRender();
            OnImGuiRender();

            p_window->OnUpdate();

            timestep = timer.EndTimerAndReturnSeconds();
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_layerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_layerStack.PushOverlay(layer);
    }

    void Application::OnEvent(Event& event)
    {
        if (event.GetEventType() == EventType::WindowClose)
            m_running = false;
        p_window->OnEvent(event);

        for (auto it = m_layerStack.end(); it != m_layerStack.begin(); )
        {
            (*--it)->OnEvent(event);
            if (event.Handled)
                break;
        }
    }

    void Application::OnUpdate(float timestep)
    {
        m_input.OnUpdate();
        for (Layer* layer : m_layerStack) {
            layer->OnUpdate(timestep);
        }
    }

    void Application::OnRender()
    {
        m_renderer.SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        m_renderer.Clear();
        for (Layer* layer : m_layerStack) {
            layer->OnRender(m_renderer);
        }
    }

    void Application::OnImGuiRender()
    {
        m_imGuiLayer.Begin();
        for (Layer* layer : m_layerStack) {
            layer->OnImGuiRender();
        }
        m_imGuiLayer.End();
    }
}
