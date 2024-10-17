#include "ngpch.h"
#include "ImGuiLayer.h"

#include "core/Window.h"

namespace Nigozi
{
	void ImGuiLayer::OnAttach()
	{
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

        const char* glsl_version = "#version 130";

        ImGui_ImplGlfw_InitForOpenGL(Global::windowData.NativeWindow, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();
	}

    void ImGuiLayer::OnEvent(Event& event)
    {
        EventDispatcher disptacher(event);
        disptacher.Dispatch<MouseButtonPressedEvent>(std::bind(&ImGuiLayer::OnMouseOnUI, this, std::placeholders::_1));
    }

	void ImGuiLayer::OnDetach()
	{
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

    void ImGuiLayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    bool ImGuiLayer::OnMouseOnUI(MouseButtonPressedEvent& event)
    {
        return ImGui::GetIO().WantCaptureMouse;
    }
}