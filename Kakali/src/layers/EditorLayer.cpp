#include "EditorLayer.h"

bool EditorLayer::s_showDemoWindow = false;

EditorLayer::EditorLayer(Nigozi::FrameBuffer* viewportBuffer)
    :m_mouseOldPosition(0.0f), m_viewportPosition(0.0f), m_windowPosition(0.0f),
    m_editorCamera(viewportBuffer->GetWidth() / (float)viewportBuffer->GetHeight(), 5.0f)
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
    entity2.AddComponent<Nigozi::SpriteRendererComponent>("src/res/textures/Player.png", glm::vec2{ 0, 0 });
}

void EditorLayer::OnEvent(Nigozi::Event& event)
{
    Nigozi::EventDispatcher dispatcher(event);
    if (m_viewportHovered) {
        m_editorCamera.OnEvent(event);
    }
    dispatcher.Dispatch<Nigozi::MouseButtonPressedEvent>(std::bind(&EditorLayer::OnSelectMouseButtonPressed, this, std::placeholders::_1));
    dispatcher.Dispatch<Nigozi::MouseButtonPressedEvent>(std::bind(&EditorLayer::OnMoveMouseButtonPressed, this, std::placeholders::_1));
    dispatcher.Dispatch<Nigozi::MouseMovedEvent>(std::bind(&EditorLayer::OnMouseMoved, this, std::placeholders::_1));
    dispatcher.Dispatch<Nigozi::MouseMovedEvent>(std::bind(&EditorLayer::OnRotateMouseMoved, this, std::placeholders::_1));
    dispatcher.Dispatch<Nigozi::MouseButtonReleasedEvent>(std::bind(&EditorLayer::OnMouseButtonReleased, this, std::placeholders::_1));
}

void EditorLayer::OnUpdate(float timestep)
{
    if (m_viewportHovered) {
        m_editorCamera.OnUpdate(timestep);
    }
    m_scene->OnEditorUpdate(timestep);
}

void EditorLayer::OnRender()
{
    m_editorCamera.OnResize(m_viewportSize.x, m_viewportSize.y);
    m_editorCamera.OnRender();
    m_scene->OnEditorRender();

    Nigozi::Renderer2D::Flush();
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    if (m_selectionContext != entt::null) {
        auto entity = Nigozi::Entity(m_selectionContext, m_scene.get());
        if (!entity.HasComponent<Nigozi::SpriteRendererComponent>()) {
            goto endSelectionGizmoRender;
        }

        auto& transform = entity.GetComponent<Nigozi::TransformComponent>();
        auto& sprite = entity.GetComponent<Nigozi::SpriteRendererComponent>();

        glm::vec2 spriteSize = sprite.Sprite->GetSize();
        float aspectX = spriteSize.x / spriteSize.y;

        glm::vec2 scaleWithSprite = glm::vec2(transform.Scale.x * aspectX, transform.Scale.y);

        Nigozi::Renderer2D::DrawRotatedQuad(
            transform.Position, 
            scaleWithSprite,
            glm::radians(transform.Rotation),
            Nigozi::Renderer2D::GetData()->Textures[0],
            glm::vec4(1.0f)
        );
    }
endSelectionGizmoRender:

    auto cameraView = m_scene->m_Registry.view<Nigozi::TransformComponent, Nigozi::CameraComponent>();
    for (auto [entityHandle, transform, camera] : cameraView.each()) {
        float aspect = m_editorCamera.GetCamera().GetAspect();
        glm::vec4 color(0.4f, 0.6f, 1.0f, 1.0f);
        if (!camera.Current) {
            color = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
        }
        Nigozi::Renderer2D::DrawRotatedQuad(
            transform.Position,
            glm::vec2(camera.Zoom * aspect, camera.Zoom),
            glm::radians(transform.Rotation),
            Nigozi::Renderer2D::GetData()->Textures[0],
            color
        );
    }
    Nigozi::Renderer2D::Flush();
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

void EditorLayer::OnImGuiRender()
{
    DockViewportWithMenuBar();
    ShowSceneHierarchy();
    ShowInspector();
    ShowViewport();
    m_scene->OnEditorImGuiRender();
    if (s_showDemoWindow) {
        ImGui::ShowDemoWindow(&s_showDemoWindow);
    }
}

bool EditorLayer::OnSelectMouseButtonPressed(Nigozi::MouseButtonPressedEvent& event)
{
    if (m_tool != Tool::SELECT || !m_viewportHovered) {
        return false;
    }
    if (event.GetButton() == GLFW_MOUSE_BUTTON_1) {
        glm::vec2 viewportRelPos = m_viewportPosition - m_windowPosition;
        glm::vec2 mousePosition = m_editorCamera.GetCamera().GetMousePositionWorldSpace(Nigozi::Input::GetMousePosition() - viewportRelPos, *(glm::vec2*)&m_viewportSize);

        // Nigozi::Renderer2D::DrawQuad(mousePosition, glm::vec2(0.1f), Nigozi::Renderer2D::GetData()->Textures[0], glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

        auto view = m_scene->m_Registry.view<Nigozi::TransformComponent, Nigozi::SpriteRendererComponent>();
        view.use<Nigozi::SpriteRendererComponent>();

        bool selected = false;
        for (auto [entityHandle, transform, sprite] : view.each()) {
            glm::vec2 delta = mousePosition - transform.Position;

            // Scale along with the sprite size to ensure we can
            // click on anywhere on the visible sprite to select
            glm::vec2 absScale = glm::abs(transform.Scale);
            glm::vec2 spriteSize = sprite.Sprite->GetSize();
            float aspectX = spriteSize.x / spriteSize.y;
            glm::vec2 scaleWithSprite = glm::vec2(absScale.x * aspectX, absScale.y);

            // Transform the point into rectangle space
            float rotation = glm::radians(-transform.Rotation);
            glm::vec2 normal(glm::cos(rotation), glm::sin(-rotation));
            glm::vec2 rotated(delta.x * normal.x - delta.y * normal.y,
                delta.x * normal.y + delta.y * normal.x);

            // Nigozi::Renderer2D::DrawRotatedQuad(transform.Position + rotated, glm::vec2(0.1f), rotation, Nigozi::Renderer2D::GetData()->Textures[0], glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

            // Check collision in rectangle local space
            glm::vec2 halfSize = scaleWithSprite / 2.0f;
            bool pointInRect = (glm::abs(rotated.x) <= halfSize.x) &&
                               (glm::abs(rotated.y) <= halfSize.y);

            if (pointInRect) {
                m_selectionContext = entityHandle;
                m_movingSelectionContext = entityHandle;
                selected = true;
            }
        }

        if (!selected) {
            m_selectionContext = entt::null;
        }
    }
    return false;
}

bool EditorLayer::OnMouseButtonReleased(Nigozi::MouseButtonReleasedEvent& event)
{
    if (event.GetButton() == GLFW_MOUSE_BUTTON_1) {
        m_movingSelectionContext = entt::null;
    }
    return false;
}

bool EditorLayer::OnMoveMouseButtonPressed(Nigozi::MouseButtonPressedEvent& event)
{
    if (m_tool != Tool::MOVE && m_tool != Tool::ROTATE || !m_viewportHovered) {
        return false;
    }
    if (event.GetButton() == GLFW_MOUSE_BUTTON_1) {
        m_movingSelectionContext = m_selectionContext;
    }
    return false;
}

bool EditorLayer::OnMouseMoved(Nigozi::MouseMovedEvent& event)
{
    if (m_tool != Tool::SELECT && m_tool != Tool::MOVE || m_movingSelectionContext == entt::null) {
        m_mouseOldPosition = glm::vec2(event.GetX(), event.GetY());
        return false;
    }
    auto& transform = Nigozi::Entity(m_movingSelectionContext, m_scene.get()).GetComponent<Nigozi::TransformComponent>();

    glm::vec2 viewportRelPos = m_viewportPosition - m_windowPosition;
    glm::vec2 mouseScreenPosition = glm::vec2(event.GetX(), event.GetY());
    glm::vec2 mousePosition = m_editorCamera.GetCamera().GetMousePositionWorldSpace(glm::vec2(event.GetX(), event.GetY()) - viewportRelPos, *(glm::vec2*)&m_viewportSize);

    glm::vec2 mouseOldPosition = m_editorCamera.GetCamera().GetMousePositionWorldSpace(m_mouseOldPosition - viewportRelPos, *(glm::vec2*)&m_viewportSize);

    glm::vec2 mouseDelta = mousePosition - mouseOldPosition;

    transform.Position += mouseDelta;

    m_mouseOldPosition = mouseScreenPosition;

    return false;
}

bool EditorLayer::OnRotateMouseMoved(Nigozi::MouseMovedEvent& event)
{
    if (m_tool != Tool::ROTATE || m_movingSelectionContext == entt::null) {
        return false;
    }
    auto& transform = Nigozi::Entity(m_movingSelectionContext, m_scene.get()).GetComponent<Nigozi::TransformComponent>();

    glm::vec2 viewportRelPos = m_viewportPosition - m_windowPosition;
    glm::vec2 mouseScreenPosition = glm::vec2(event.GetX(), event.GetY());
    glm::vec2 mousePosition = m_editorCamera.GetCamera().GetMousePositionWorldSpace(glm::vec2(event.GetX(), event.GetY()) - viewportRelPos, *(glm::vec2*)&m_viewportSize);
    glm::vec2 delta = mousePosition - transform.Position;
    
    if (delta.x == 0.0f) {
        return false;
    }

    float angle = glm::degrees(glm::atan(-delta.y / delta.x));
    if (delta.x < 0.0f) {
        angle -= 180.0f;
    }
    if (angle < 0.0f) {
        angle += 360.0f;
    }

    transform.Rotation = angle;

    return false;
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
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Demo Window", NULL, &s_showDemoWindow);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void EditorLayer::ShowSceneHierarchy()
{
    ImGui::Begin("Scene Hierarchy");
    m_windowPosition = glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
    ShowAddNodeModal();
    ImGui::SameLine();
    if (ImGui::Button("X") && m_selectionContext != entt::null) {
        ImGui::OpenPopup("Delete?");
    }

    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Are you sure you want to delete this node?");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            Nigozi::Entity entity(m_selectionContext, m_scene.get());
            entity.Destroy();
            m_selectionContext = entt::null;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    auto view = m_scene->m_Registry.view<Nigozi::NameComponent>();
    for (const auto entityHandle : view) {
        Nigozi::Entity entity(entityHandle, m_scene.get());
        auto& nameComponent = entity.GetComponent<Nigozi::NameComponent>();
        ImGuiTreeNodeFlags flags = ((m_selectionContext == entityHandle) ? ImGuiTreeNodeFlags_Selected : 0)
            | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.GetHandle(), flags, nameComponent.Name.c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
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

        if (entity.HasComponent<Nigozi::CameraComponent>() &&
            ImGui::TreeNodeEx((void*)(typeid(Nigozi::CameraComponent).hash_code() + (size_t)m_selectionContext),
                ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
            auto& camera = entity.GetComponent<Nigozi::CameraComponent>();
            ImGui::DragFloat("Zoom", &camera.Zoom, 0.05f, 0.01f);
            if (ImGui::Checkbox("Current", &camera.Current)) {
                auto cameraView = m_scene->m_Registry.view<Nigozi::CameraComponent>();
                for (auto [entityHandle, otherCamera] : cameraView.each()) {
                    if (entityHandle == entity.GetHandle()) {
                        continue;
                    }
                    otherCamera.Current = false;
                }
            }

            ImGui::TreePop();
            ImGui::Separator();
        }

        if (entity.HasComponent<Nigozi::SpriteRendererComponent>() &&
            ImGui::TreeNodeEx((void*)(typeid(Nigozi::SpriteRendererComponent).hash_code() + (size_t)m_selectionContext),
                ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer")) {
            auto& sprite = entity.GetComponent<Nigozi::SpriteRendererComponent>();

            ImGuiTableFlags flags =
                ImGuiTableFlags_SizingFixedFit |
                ImGuiTableFlags_Hideable;

            // TRANSFORM TABLE
            float availableSizeX = ImGui::GetContentRegionAvail().x;
            // UV coordinates are often (0.0f, 0.0f) and (1.0f, 1.0f) to display an entire textures.
            // Here are trying to display only a 32x32 pixels area of the texture, hence the UV computation.
            // Read about UV coordinates here: https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
            ImGui::PushID((void*)(typeid(Nigozi::Texture).hash_code() + (size_t)m_selectionContext));
            ImVec2 size = ImVec2(availableSizeX / 2.0f - 10.0f, availableSizeX / 2.0f - 10.0f);                     // Size of the image we want to make visible
            ImVec2 uv0 = ImVec2(0.0f, 1.0f);                            // UV coordinates for lower-left
            ImVec2 uv1 = ImVec2(1.0f, 0.0f);    
            ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
            if (ImGui::ImageButton("Sprite", sprite.SpriteTexture->GetRendererID(), size, uv0, uv1, bg_col, tint_col)) {
                std::filesystem::path result = Nigozi::FileDialogue::OpenFileDialog("png;jpg;jpeg");
                if (!result.empty()) {
                    std::string path = result.string();

                    sprite.Sprite.reset();
                    sprite.SpriteTexture.reset();
                    sprite.SpriteTexture = std::make_shared<Nigozi::Texture>(path);
                    sprite.Sprite = std::make_shared<Nigozi::SubTexture>(sprite.SpriteTexture, sprite.SpriteTexture->GetSize());
                }
            }

            ImGui::ColorEdit4("Color", (float*)&sprite.Color);
            int zorder = sprite.ZOrder;
            ImGui::SliderInt("Z Order", &zorder, -127, 127);
            sprite.ZOrder = (int16_t)zorder;
            
            ImGui::PopID();
            glm::vec2 seperatorFloats = sprite.Sprite->GetTextureSize() / sprite.Sprite->GetSize();
            uint32_t seperators[2] = { (uint32_t)seperatorFloats.x, (uint32_t)seperatorFloats.y };
            uint32_t lastSeperators[2] = { seperators[0], seperators[1] };
            ImGui::DragInt2("Seperators", (int*)seperators, 1.0f, 1, 0x7fffffff);
            if (seperators[0] != lastSeperators[0] || seperators[1] != lastSeperators[1]) {
                glm::vec2& texSize = sprite.Sprite->GetTextureSize();
                glm::vec2 size(texSize.x / seperators[0], texSize.y / seperators[1]);
                sprite.Sprite->SetSubTexture(size, 0, 0);
            }
            uint32_t slots[2] = { sprite.Sprite->GetSlotX(), sprite.Sprite->GetSlotY() };
            uint32_t lastSlots[2] = { slots[0], slots[1] };
            ImGui::DragInt2("Slot", (int*)&slots, 1.0f, 0, 0x7fffffff);
            if (slots[0] != lastSlots[0] || slots[1] != lastSlots[1]) {
                sprite.Sprite->SetSlot(slots[0], slots[1]);
            }
            ImGui::TreePop();
            ImGui::Separator();
        }

        if (entity.HasComponent<Nigozi::AudioStreamPlayerComponent>() &&
            ImGui::TreeNodeEx((void*)(typeid(Nigozi::AudioStreamPlayerComponent).hash_code() + (size_t)m_selectionContext),
                ImGuiTreeNodeFlags_DefaultOpen, "Audio Stream Player")) {
            auto& audio = entity.GetComponent<Nigozi::AudioStreamPlayerComponent>();

            if (ImGui::Button("Choose")) {
                std::filesystem::path result = Nigozi::FileDialogue::OpenFileDialog("wav,mp3,flac");
                NG_CORE_LOG_INFO("New audio path: {}", result.string());
                if (!result.empty()) {
                    Nigozi::Audio* newAudio = nullptr;
                    if (audio.AudioHandle) {
                        newAudio = Nigozi::AudioEngine::LoadAudioFromFile(result, audio.AudioHandle->GetAudioGroupName());
                    }
                    else {
                        newAudio = Nigozi::AudioEngine::LoadAudioFromFile(result);
                    }
                    NG_CORE_LOG_INFO("New audio: {}", (size_t)newAudio);
                    if (newAudio) {
                        if (audio.AudioHandle) {
                            Nigozi::AudioEngine::UnloadAudio(audio.AudioHandle);
                        }
                        audio.AudioHandle = newAudio;
                        audio.AudioHandle->SetVolume(audio.Volume);
                        NG_CORE_LOG_INFO("Set audio: {}", (size_t)audio.AudioHandle);
                    }
                    else {
                        NG_CORE_LOG_ERROR("Audio with path: {} couldn't load!", result.string());
                    }
                }
                else {
                    NG_CORE_LOG_ERROR("Couldn't get audio path: {}", result.string());
                }
            }
            std::filesystem::path audioPath("empty");
            if (audio.AudioHandle) {
                NG_CORE_LOG_INFO(audio.AudioHandle->GetFilePath().string());
                audioPath = audio.AudioHandle->GetFilePath();
            }
            std::string audioPathString = audioPath.string();
            if (audioPathString.size() > 32) {
                audioPathString = audioPathString.substr(audioPathString.size() - 32);
            }
            ImGui::Text(audioPathString.c_str());
            if (audio.AudioHandle) {
                if (ImGui::DragFloat("Volume", &audio.Volume, 0.2f)) {
                    audio.AudioHandle->SetVolume(audio.Volume);
                }
                bool lastPlayingVal = audio.AudioHandle->IsPlaying();
                bool playingVal = lastPlayingVal;
                if (ImGui::Checkbox("Playing", &playingVal)) {
                    if (playingVal != lastPlayingVal) {
                        if (playingVal) {
                            audio.AudioHandle->Play();
                        }
                        else {
                            audio.AudioHandle->Stop();
                        }
                    }
                }
            }

            ImGui::TreePop();
            ImGui::Separator();
        }

        if (ImGui::TreeNodeEx((void*)(typeid(Nigozi::TransformComponent).hash_code() + (size_t)m_selectionContext),
            ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
            auto& transform = entity.GetComponent<Nigozi::TransformComponent>();
            
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2{ 10.0f, 4.0f });
            
            ImGuiTableFlags flags = 
                ImGuiTableFlags_SizingFixedFit | 
                ImGuiTableFlags_Resizable | 
                ImGuiTableFlags_Hideable;

            // TRANSFORM TABLE
            ImGui::BeginTable("transform_table", 2, flags);

            ImGui::TableSetupColumn("##TRANSFORM_LABEL", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##TRANSFORM_PARAMS", ImGuiTableColumnFlags_WidthStretch);
            // --POSITION ROW
            ImGui::TableNextRow();

            // --(0)POSITION TITLE COLUMN
            // ImGui::SetColumnWidth(0, 100);
            ImGui::TableNextColumn();
            ImGui::Text("Position");
            
            ImGui::TableNextColumn();

            // --(1)POSITION TABLE COLUMN
            ImGuiTableFlags innerFlags = 
                ImGuiTableFlags_NoPadInnerX | 
                ImGuiTableFlags_NoPadOuterX | 
                ImGuiTableFlags_Hideable;
            ImGui::BeginTable("position_value_table", 2, innerFlags);

            ImGui::TableSetupColumn("##TRANSFORM_POS_X", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("##TRANSFORM_POS_Y", ImGuiTableColumnFlags_WidthStretch);

            // ----POSITION ROW
            ImGui::TableNextRow();

            // ----(0)POSITION_X COLUMN
            ImGui::TableNextColumn();
            ImGui::PushID(&transform.Position);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 10.0f, 0.0f });
            ImGui::Text("X");
            ImGui::SameLine();

            ImGui::PushItemWidth(-10.0f);
            ImGui::DragFloat("##X", &transform.Position.x, 0.1f, -INFINITY, INFINITY, "%.2f");
            ImGui::PopItemWidth();

            // ----(1)POSITION_Y COLUMN
            ImGui::TableNextColumn();
            ImGui::Text("Y");
            ImGui::SameLine();

            ImGui::PushItemWidth(-10.0f);
            ImGui::DragFloat("##Y", &transform.Position.y, 0.1f);
            ImGui::PopItemWidth();

            ImGui::PopID();

            // --(1)POSITION TABLE END COLUMN
            ImGui::EndTable();

            // --SCALE ROW
            ImGui::TableNextRow();

            // --(0)SCALE TITLE COLUMN
            ImGui::TableNextColumn();
            ImGui::Text("Scale");

            ImGui::TableNextColumn();

            // --(1)SCALE TABLE COLUMN
            ImGui::BeginTable("scale_value_table", 2, innerFlags);

            ImGui::TableSetupColumn("##TRANSFORM_SCALE_X", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("##TRANSFORM_SCALE_Y", ImGuiTableColumnFlags_WidthStretch);

            // ----SCALE ROW
            ImGui::TableNextRow();

            // ----(0)SCALE_X COLUMN
            ImGui::TableNextColumn();
            ImGui::PushID(&transform.Scale);
            ImGui::Text("X");
            ImGui::SameLine();

            ImGui::PushItemWidth(-10.0f);
            ImGui::DragFloat("##X", &transform.Scale.x, 0.1f);
            ImGui::PopItemWidth();

            // ----(1)SCALE_Y COLUMN
            ImGui::TableNextColumn();
            ImGui::Text("Y");
            ImGui::SameLine();

            ImGui::PushItemWidth(-10.0f);
            ImGui::DragFloat("##Y", &transform.Scale.y, 0.1f);
            ImGui::PopItemWidth();
            ImGui::PopStyleVar();

            ImGui::PopID();

            // --(1)SCALE TABLE END COLUMN
            ImGui::EndTable();

            // ----SCALE ROW
            ImGui::TableNextRow();

            // ----(0)SCALE_X COLUMN
            ImGui::TableNextColumn();
            ImGui::PushID(&transform.Rotation);
            ImGui::Text("Rotation");

            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-10.0f);
            ImGui::DragFloat("##ROTATION", &transform.Rotation, 0.1f, -360.0f, 360.0f, "%.2f", ImGuiSliderFlags_WrapAround);
            ImGui::PopItemWidth();

            ImGui::PopID();

            // TRANSFORM TABLE END
            ImGui::EndTable();

            ImGui::PopStyleVar();
            ImGui::PopStyleVar();

            ImGui::TreePop();
            ImGui::Separator();
        }
    }
    ImGui::End();
}

void EditorLayer::ShowAddNodeModal()
{
    if (ImGui::Button("Add Node")) {
        ImGui::OpenPopup("Add Node...");
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Add Node...", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("This is where you can add nodes");

        constexpr size_t SPRITE_RENDERER = 0;
        constexpr size_t AUDIO_STREAM_PLAYER = 1;
        constexpr size_t CAMERA = 2;
        static std::array<std::string, 3> items = {
            "Sprite Renderer",
            "Audio Stream Player",
            "Camera"
        };
        static size_t itemSelectedIndex = -1;

        const char* empty = ".";
        static char* preview;
        if (!preview) {
            preview = (char*)empty;
        }

        if (ImGui::BeginCombo("Nodes", preview, 0)) {
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
                Nigozi::Entity entity = m_scene->CreateEntity("New Node", "Empty");
                AddComponent<Nigozi::SpriteRendererComponent>(entity, itemSelectedIndex == SPRITE_RENDERER);
                AddComponent<Nigozi::AudioStreamPlayerComponent>(entity, itemSelectedIndex == AUDIO_STREAM_PLAYER);
                AddComponent<Nigozi::CameraComponent>(entity, itemSelectedIndex == CAMERA);
                if (itemSelectedIndex == CAMERA) {
                    auto cameraView = m_scene->m_Registry.view<Nigozi::CameraComponent>();
                    for (auto [entityHandle, otherCamera] : cameraView.each()) {
                        if (entityHandle != entity.GetHandle()) {
                            otherCamera.Current = false;
                        }
                    }
                }
                m_selectionContext = entity.GetHandle();
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void EditorLayer::ShowViewport()
{
    // TODO: Make multiple tools: Select, Move, Rotate
    bool buttonHovered = false;
    ImGui::Begin("Viewport");
    if (ImGui::Button("Select Tool")) {
        buttonHovered = true;
        m_tool = Tool::SELECT;
    }
    if (ImGui::IsItemHovered()) buttonHovered = true;
    ImGui::SameLine();
    if (ImGui::Button("Move Tool")) {
        buttonHovered = true;
        m_tool = Tool::MOVE;
    }
    if (ImGui::IsItemHovered()) buttonHovered = true;
    ImGui::SameLine();
    if (ImGui::Button("Rotate Tool")) {
        buttonHovered = true;
        m_tool = Tool::ROTATE;
    }
    if (ImGui::IsItemHovered()) buttonHovered = true;

    m_viewportSize = ImGui::GetContentRegionAvail();

    ImVec2 viewportFrameSize = ImGui::GetContentRegionMax();
    ImVec2 padding{ viewportFrameSize.x - m_viewportSize.x, viewportFrameSize.y - m_viewportSize.y };
    padding.y += ImGui::GetFrameHeight();

    m_viewportPosition = glm::vec2(ImGui::GetWindowPos().x + padding.x, ImGui::GetWindowPos().y + padding.y);

    ImGui::Image((uint64_t)(p_viewportBuffer->GetColorAttachment()), m_viewportSize, ImVec2(0, 1), ImVec2(1, 0));
    m_viewportHovered = ImGui::IsWindowHovered() && !buttonHovered;
    ImGui::End();
}
