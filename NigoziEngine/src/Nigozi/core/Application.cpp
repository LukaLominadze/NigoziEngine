#include "ngpch.h"

#include "Application.h"
#include "glcore/Renderer2D.h"

namespace Nigozi
{
    bool Application::s_running = false;

    Application::Application(const ApplicationProps& props)
        :m_input(Input())
    {
        p_window = new Window(props.Title, props.Width, props.Height, props.Fullscreen);
        // Polled events will be sent to the OnEvent function in the application
        p_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        p_window->SetVSync(props.VSync);

        PushOverlay(&m_imGuiLayer);

        Renderer2D::Initialize();
        Renderer2D::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        s_running = true;

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

        s_running = true;

        while (s_running)
        {
            timer.StartTimerAndReturnSeconds();

            OnUpdate(timestep);
            OnRender();
            OnImGuiRender();

            p_window->OnUpdate();

            timestep = timer.EndTimerAndReturnSeconds();
        }
    }

    void Application::Close()
    {
        s_running = false;
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

    void Application::OnEvent(Event& event)
    {
        if (event.GetEventType() == EventType::WindowClose)
        {
            s_running = false;
            return;
        }

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
}
