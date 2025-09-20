#include "EditorLayer.h"

EditorLayer::EditorLayer(Nigozi::FrameBuffer* viewportBuffer)
{
    p_viewportBuffer = viewportBuffer;
    m_sceneManager = Nigozi::SceneManager("Sample", [this]() { return std::make_shared<Nigozi::Scene>(&m_sceneManager); });
    m_sceneManager.OnAttach();
    m_scene = m_sceneManager.GetCurrentScene();
    m_scene->OnAttach();

    Nigozi::Entity entity1 = m_scene->CreateEntity("Mario", "Player");
    entity1.GetComponent<Nigozi::TransformComponent>().Rotation = 30.0f;
    entity1.AddComponent<Nigozi::SpriteRendererComponent>("src/Nigozi/res/textures/luigi.png", glm::vec2{ 0, 0 });

    Nigozi::Entity entity2 = m_scene->CreateEntity("Nigozi", "Entity");
    //entity2.AddComponent<Nigozi::SpriteRendererComponent>("src/Nigozi/res/textures/logo.png", glm::vec2{ 0, 0 });
}

void EditorLayer::OnUpdate(float timestep)
{
    m_scene->OnUpdate(timestep);
}

void EditorLayer::OnRender()
{
    // A test square
    //Nigozi::Renderer2D::DrawQuad(glm::vec3(0, 0, -1), glm::vec2(1, 1), nullptr, glm::vec4(0.1f, 0.3f, 0.8f, 1.0f));
    m_scene->OnRender();
}

void EditorLayer::OnImGuiRender()
{
    DockViewportWithMenuBar();
    ShowSceneHierarchy();
    ShowInspector();

    ImGui::Begin("Viewport");
    m_viewportSize = ImGui::GetContentRegionAvail();
    ImGui::Image((uint64_t)(p_viewportBuffer->GetColorAttachment()), m_viewportSize, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

void EditorLayer::DockViewportWithMenuBar()
{
    // READ THIS !!!
    // TL;DR; this demo is more complicated than what most users you would normally use.
    // If we remove all options we are showcasing, this demo would become:
    //     void ShowExampleAppDockSpace()
    //     {
    //         ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    //     }
    // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
    // In this specific demo, we are not using DockSpaceOverViewport() because:
    // - (1) we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
    // - (2) we allow the host window to have padding (when opt_padding == true)
    // - (3) we expose many flags and need a way to have them visible.
    // - (4) we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport()
    //      in your code, but we don't here because we allow the window to be floating)


    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    static bool open = true;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    //----------------------MENU BAR------------------------
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                Nigozi::Application::Close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
            if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
            if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, &open != NULL))
                open = false;
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void EditorLayer::ShowSceneHierarchy()
{
    ImGui::Begin("Scene Hierarchy");
    auto view = m_scene->m_Registry.view<Nigozi::NameComponent>();
    if (ImGui::Button("+")) {
        Nigozi::Entity newEntity = m_scene->CreateEntity("New Object", "Empty");
    }
    for (const auto entityHandle : view) {
        Nigozi::Entity entity(entityHandle, m_scene.get());
        auto& nameComponent = entity.GetComponent<Nigozi::NameComponent>();
        ImGuiTreeNodeFlags flags = ((m_selectionContext == entityHandle) ? ImGuiTreeNodeFlags_Selected : 0)
            | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.GetHandle(), flags, nameComponent.Name.c_str());
        if (ImGui::IsItemClicked()) {
            if (m_selectionContext == entityHandle) {
                m_selectionContext = entt::null;
            }
            else {
                m_selectionContext = entityHandle;
            }
        }

        if (opened) {
            ImGui::TreePop();
        }
    }
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
        m_selectionContext = entt::null;
    }
    ImGui::End();
}

void EditorLayer::ShowInspector()
{
    ImGui::Begin("Inspector");
    if (m_selectionContext != entt::null) {
        Nigozi::Entity entity(m_selectionContext, m_scene.get());

        auto& nameComponent = entity.GetComponent<Nigozi::NameComponent>();
        auto& tagComponent = entity.GetComponent<Nigozi::TagComponent>();
        ImGui::InputText("Name", nameComponent.Name.data(), nameComponent.Name.capacity());
        ImGui::InputText("Tag", tagComponent.Tag.data(), tagComponent.Tag.capacity());

        if (ImGui::TreeNodeEx((void*)(typeid(Nigozi::TransformComponent).hash_code() + (size_t)m_selectionContext),
            ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
            auto& transform = entity.GetComponent<Nigozi::TransformComponent>();
            ImGui::Text("Position");
            ImGui::PushID(&transform.Position);
            ImGui::DragFloat("X", &transform.Position.x, 0.1f);
            //ImGui::SameLine();
            ImGui::DragFloat("Y", &transform.Position.y, 0.1f);
            ImGui::PopID();

            ImGui::Text("Scale");
            ImGui::PushID(&transform.Scale);
            ImGui::DragFloat("X", &transform.Scale.x, 0.1f);
            //ImGui::SameLine();
            ImGui::DragFloat("Y", &transform.Scale.y, 0.1f);
            ImGui::PopID();

            ImGui::DragFloat("Rotation", &transform.Rotation);

            ImGui::TreePop();
        }

        if (entity.HasComponent<Nigozi::SpriteRendererComponent>() &&
            ImGui::TreeNodeEx((void*)(typeid(Nigozi::SpriteRendererComponent).hash_code() + (size_t)m_selectionContext),
                ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer")) {
            auto& sprite = entity.GetComponent<Nigozi::SpriteRendererComponent>();
            ImGui::ColorEdit4("Color", (float*)&sprite.Color);
            int zorder = sprite.ZOrder;
            ImGui::SliderInt("Z Order", &zorder, -127, 127);
            sprite.ZOrder = (int16_t)zorder;

            ImGui::TreePop();
        }

        if (entity.HasComponent<Nigozi::AudioStreamPlayerComponent>() &&
            ImGui::TreeNodeEx((void*)(typeid(Nigozi::AudioStreamPlayerComponent).hash_code() + (size_t)m_selectionContext),
                ImGuiTreeNodeFlags_DefaultOpen, "Audio Stream Player")) {
            auto& audio = entity.GetComponent<Nigozi::AudioStreamPlayerComponent>();

            ImGui::TreePop();
        }

        ShowAddComponentModal();
    }
    ImGui::End();
}

void EditorLayer::ShowAddComponentModal()
{
    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("Add Component...");
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Add Component...", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("This is where you can add components");

        constexpr size_t SPRITE_RENDERER = 0;
        constexpr size_t AUDIO_STREAM_PLAYER = 1;
        static std::array<std::string, 2> items = {
            "Sprite Renderer",
            "Audio Stream Player"
        };
        static size_t itemSelectedIndex = -1;

        const char* empty = ".";
        static char* preview;
        if (!preview) {
            preview = (char*)empty;
        }

        if (ImGui::BeginCombo("Components", preview, 0)) {
            for (size_t i = 0; i < items.size(); i++) {
                const bool isSelected = (itemSelectedIndex == i);
                if (ImGui::Selectable(items[i].c_str(), isSelected)) {
                    itemSelectedIndex = i;
                    preview = items[i].data();
                }

                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Add")) {
            if (itemSelectedIndex != -1) {
                Nigozi::Entity entity(m_selectionContext, m_scene.get());
                AddComponent<Nigozi::SpriteRendererComponent>(entity, itemSelectedIndex == SPRITE_RENDERER);
                AddComponent<Nigozi::AudioStreamPlayerComponent>(entity, itemSelectedIndex == AUDIO_STREAM_PLAYER);
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
