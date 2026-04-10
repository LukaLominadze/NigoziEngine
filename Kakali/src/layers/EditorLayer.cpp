#include "EditorLayer.h"

bool EditorLayer::s_showDemoWindow = false;

EditorLayer::EditorLayer(Nigozi::FrameBuffer* viewportBuffer)
    :m_mouseOldPosition(0.0f), m_viewportPosition(0.0f), m_windowPosition(0.0f),
    m_editorCamera(viewportBuffer->GetWidth() / (float)viewportBuffer->GetHeight(), 5.0f)
{
    m_currentContext = std::make_shared<Nigozi::SceneTree>();
    m_sceneTreeContexts.push_back(m_currentContext);

    p_viewportBuffer = viewportBuffer;
}

void EditorLayer::OnEvent(Nigozi::Event& event)
{
    Nigozi::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Nigozi::MouseButtonPressedEvent>(std::bind(&EditorLayer::OnSelectMouseButtonPressed, this, std::placeholders::_1));
    dispatcher.Dispatch<Nigozi::MouseButtonPressedEvent>(std::bind(&EditorLayer::OnMoveMouseButtonPressed, this, std::placeholders::_1));
    dispatcher.Dispatch<Nigozi::MouseMovedEvent>(std::bind(&EditorLayer::OnMouseMoved, this, std::placeholders::_1));
    dispatcher.Dispatch<Nigozi::MouseMovedEvent>(std::bind(&EditorLayer::OnRotateMouseMoved, this, std::placeholders::_1));
    dispatcher.Dispatch<Nigozi::MouseButtonReleasedEvent>(std::bind(&EditorLayer::OnMouseButtonReleased, this, std::placeholders::_1));
    if (m_editorState == EditorState::EDIT) {
        if (m_viewportHovered) {
            m_editorCamera.OnEvent(event);
        }
    }
    else {
        m_currentContext->OnEvent(event);
    }
}

void EditorLayer::OnUpdate(float timestep)
{
    if (m_editorState == EditorState::EDIT) {
        if (m_viewportHovered) {
            m_editorCamera.OnUpdate(timestep);
        }
        m_currentContext->OnEditorUpdate(timestep);
    }
    else {
        m_currentContext->OnUpdate(timestep);
    }
}

void EditorLayer::OnRender()
{
    if (m_editorState == EditorState::EDIT) {
        m_editorCamera.OnResize(m_viewportSize.x, m_viewportSize.y);
        m_editorCamera.OnRender();
        m_currentContext->OnEditorRender();
    }
    else {
        m_currentContext->OnRender();
        GLCall(glViewport(0, 0, m_viewportSize.x, m_viewportSize.y));
    }

    Nigozi::Renderer2D::Flush();
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    if (m_selectionContext != entt::null) {
        auto entity = Nigozi::Entity(m_selectionContext, m_currentContext.get());
        if (!entity.HasComponent<Nigozi::SpriteRendererComponent>()) {
            goto endSelectionGizmoRender;
        }

        auto transform = m_currentContext->GetWorldSpaceTransform(entity);
        auto& sprite = entity.GetComponent<Nigozi::SpriteRendererComponent>();

        glm::vec2 spriteSize = sprite.Sprite.GetSize();
        float sizeY = spriteSize.y / sprite.Sprite.GetTextureSize().y;
        float aspectX = spriteSize.x / spriteSize.y;

        glm::vec2 scaleWithSprite = glm::vec2(transform.Scale.x * aspectX * sizeY, transform.Scale.y * sizeY);

        Nigozi::Renderer2D::DrawRotatedQuad(
            transform.Position, 
            scaleWithSprite,
            glm::radians(transform.Rotation),
            Nigozi::Renderer2D::GetData()->Textures[0],
            glm::vec4(1.0f)
        );
    }
endSelectionGizmoRender:

    auto cameraView = m_currentContext->m_Registry.view<Nigozi::TransformComponent, Nigozi::CameraComponent>();
    for (auto [entityHandle, transform, camera] : cameraView.each()) {
        auto worldTransform = m_currentContext->GetWorldSpaceTransform(Nigozi::Entity(entityHandle, m_currentContext.get()));
        glm::vec4 color(0.4f, 0.6f, 1.0f, 1.0f);
        if (!camera.Current) {
            color = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
        }
        Nigozi::Renderer2D::DrawRotatedQuad(
            worldTransform.Position,
            glm::vec2(camera.Zoom * camera.Aspect * 2.0f, camera.Zoom * 2.0f),
            glm::radians(worldTransform.Rotation),
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
    if (m_editorState == EditorState::EDIT) {
        m_currentContext->OnEditorImGuiRender();
    }
    else {
        m_currentContext->OnImGuiRender();
    }
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

        auto view = m_currentContext->m_Registry.view<Nigozi::TransformComponent, Nigozi::SpriteRendererComponent>();
        view.use<Nigozi::SpriteRendererComponent>();

        bool selected = false;
        for (auto [entityHandle, transform, sprite] : view.each()) {
            // Scale along with the sprite size to ensure we can
            // click on anywhere on the visible sprite to select
            auto worldTransform = m_currentContext->GetWorldSpaceTransform(Nigozi::Entity(entityHandle, m_currentContext.get()));
            glm::vec2 delta = mousePosition - worldTransform.Position;


            glm::vec2 absScale = glm::abs(worldTransform.Scale);
            glm::vec2 spriteSize = sprite.Sprite.GetSize();
            float sizeY = spriteSize.y / sprite.Sprite.GetTextureSize().y;
            float aspectX = spriteSize.x / spriteSize.y;
            glm::vec2 scaleWithSprite(absScale.x * aspectX * sizeY, absScale.y * sizeY);

            // Transform the point into rectangle space
            float rotation = glm::radians(-worldTransform.Rotation);
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
    auto& transform = Nigozi::Entity(m_movingSelectionContext, m_currentContext.get()).GetComponent<Nigozi::TransformComponent>();
    auto worldTransform = m_currentContext->GetWorldSpaceTransform(Nigozi::Entity(m_movingSelectionContext, m_currentContext.get()));

    glm::vec2 viewportRelPos = m_viewportPosition - m_windowPosition;
    glm::vec2 mouseScreenPosition = glm::vec2(event.GetX(), event.GetY());
    glm::vec2 mousePosition = m_editorCamera.GetCamera().GetMousePositionWorldSpace(glm::vec2(event.GetX(), event.GetY()) - viewportRelPos, *(glm::vec2*)&m_viewportSize);

    glm::vec2 mouseOldPosition = m_editorCamera.GetCamera().GetMousePositionWorldSpace(m_mouseOldPosition - viewportRelPos, *(glm::vec2*)&m_viewportSize);

    glm::vec2 mouseDelta = mousePosition - mouseOldPosition;

    // Transform the point into rectangle space
    float rotation = glm::radians(-(worldTransform.Rotation - transform.Rotation));
    glm::vec2 normal(glm::cos(rotation), glm::sin(-rotation));
    glm::vec2 rotated(mouseDelta.x * normal.x - mouseDelta.y * normal.y,
        mouseDelta.x * normal.y + mouseDelta.y * normal.x);

    transform.Position += rotated;

    m_mouseOldPosition = mouseScreenPosition;

    return false;
}

bool EditorLayer::OnRotateMouseMoved(Nigozi::MouseMovedEvent& event)
{
    if (m_tool != Tool::ROTATE || m_movingSelectionContext == entt::null) {
        return false;
    }
    auto& transform = Nigozi::Entity(m_movingSelectionContext, m_currentContext.get()).GetComponent<Nigozi::TransformComponent>();
    auto worldTransform = m_currentContext->GetWorldSpaceTransform(Nigozi::Entity(m_movingSelectionContext, m_currentContext.get()));

    glm::vec2 viewportRelPos = m_viewportPosition - m_windowPosition;
    glm::vec2 mouseScreenPosition = glm::vec2(event.GetX(), event.GetY());
    glm::vec2 mousePosition = m_editorCamera.GetCamera().GetMousePositionWorldSpace(glm::vec2(event.GetX(), event.GetY()) - viewportRelPos, *(glm::vec2*)&m_viewportSize);

    // Transform the point into rectangle space
    float rotation = glm::radians(-worldTransform.Rotation);
    glm::vec2 normal(glm::cos(rotation), glm::sin(-rotation));
    glm::vec2 rotated(mousePosition.x * normal.x - mousePosition.y * normal.y,
        mousePosition.x * normal.y + mousePosition.y * normal.x);

    glm::vec2 delta = mousePosition - worldTransform.Position;
    
    if (delta.x == 0.0f) {
        return false;
    }

    float angle = glm::degrees(glm::atan(-delta.y / delta.x));

    angle -= (worldTransform.Rotation - transform.Rotation);
    angle -= ((int)(angle / 360)) * 360.0f;

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
            if (ImGui::MenuItem("Save")) {
                std::filesystem::path savePath = Nigozi::FileDialogue::OpenSaveDialog("ngscn");
                if (!savePath.empty()) {
                    m_currentContext->SerializeScene(savePath);
                }
            }
            if (ImGui::MenuItem("Load")) {
                std::filesystem::path scenePath = Nigozi::FileDialogue::OpenFileDialog("ngscn");
                if (!scenePath.empty()) {
                    m_currentContext->ClearSceneTree();
                    m_currentContext->DeserializeScene(scenePath);
                }
            }
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

    auto view = m_currentContext->m_Registry.view<Nigozi::UUIDComponent>();
    
    if (view.empty()) {
        ImGui::Text("Add your first node");
    }
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
            Nigozi::Entity entity(m_selectionContext, m_currentContext.get());
            m_currentContext->DestroyEntity(entity);
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

    for (const auto entityHandle : view) {
        Nigozi::Entity entity(entityHandle, m_currentContext.get());
        auto& relationshipComponent = entity.GetComponent<Nigozi::RelationshipComponent>();

        if (relationshipComponent.ParentUUID.GetUUID() == Nigozi::UUID::Null) {
            DrawSceneHierarchyNode(entity);
        }
    }
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
        m_selectionContext = entt::null;
    }
    ImGui::End();
}

void EditorLayer::DrawSceneHierarchyNode(Nigozi::Entity entity)
{
    auto& nameComponent = entity.GetComponent<Nigozi::NameComponent>();

    ImGuiTreeNodeFlags flags = ((m_selectionContext == entity.GetHandle()) ? ImGuiTreeNodeFlags_Selected : 0)
        | ((entity.GetChildrenUUIDs().size() == 0) ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick);

    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.GetHandle(), flags, nameComponent.Name.c_str());

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        if (m_selectionContext == entity.GetHandle()) {
            m_selectionContext = entt::null;
        }
        else {
            m_selectionContext = entity.GetHandle();
        }
    }

    if (opened) {
        for (Nigozi::Entity child : entity.GetChildren()) {
            DrawSceneHierarchyNode(child);
        }

        ImGui::TreePop();
    }
}

void EditorLayer::ShowInspector()
{
    ImGui::Begin("Inspector");
    if (m_selectionContext != entt::null) {
        Nigozi::Entity entity(m_selectionContext, m_currentContext.get());

        auto& uuidComponent = entity.GetComponent<Nigozi::UUIDComponent>();
        auto& nameComponent = entity.GetComponent<Nigozi::NameComponent>();
        auto& tagComponent = entity.GetComponent<Nigozi::TagComponent>();
        ImGui::Text(std::to_string(uuidComponent.ID.GetUUID()).c_str());
        ImGui::InputText("Name", nameComponent.Name.data(), nameComponent.Name.capacity());
        ImGui::InputText("Tag", tagComponent.Tag.data(), tagComponent.Tag.capacity());

        if (entity.HasComponent<Nigozi::CameraComponent>() &&
            ImGui::TreeNodeEx((void*)(typeid(Nigozi::CameraComponent).hash_code() + (size_t)m_selectionContext),
                ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
            auto& camera = entity.GetComponent<Nigozi::CameraComponent>();
            ImGui::DragFloat("Zoom", &camera.Zoom, 0.05f, 0.01f);
            if (ImGui::Checkbox("Current", &camera.Current)) {
                auto cameraView = m_currentContext->m_Registry.view<Nigozi::CameraComponent>();
                for (auto [entityHandle, otherCamera] : cameraView.each()) {
                    if (entityHandle == entity.GetHandle()) {
                        continue;
                    }
                    otherCamera.Current = false;
                }
            }
            ImGui::DragFloat("Aspect", &camera.Aspect, 0.1f, 0.01f);

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
            ImVec2 size = ImVec2(availableSizeX / 2.0f - 10.0f, availableSizeX / 2.0f - 10.0f);                     // Size of the image we want to make visible
            ImVec2 uv0 = ImVec2(0.0f, 1.0f);                            // UV coordinates for lower-left
            ImVec2 uv1 = ImVec2(1.0f, 0.0f);    
            ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
            if (ImGui::ImageButton("Sprite", sprite.SpriteTexture->GetRendererID(), size, uv0, uv1, bg_col, tint_col)) {
                std::filesystem::path result = Nigozi::FileDialogue::OpenFileDialog("png;jpg;jpeg");
                if (!result.empty()) {
                    std::string path = result.string();

                    sprite.SpriteTexture.reset();
                    sprite.SpriteTexture = std::make_shared<Nigozi::Texture>(path);
                    sprite.Sprite = Nigozi::SubTexture(sprite.SpriteTexture, sprite.SpriteTexture->GetSize());
                }
            }

            ImGui::ColorEdit4("Color", (float*)&sprite.Color);
            int zorder = sprite.ZOrder;
            ImGui::SliderInt("Z Order", &zorder, -127, 127);
            sprite.ZOrder = (int16_t)zorder;
            
            glm::i32vec2 seperators = glm::i32vec2((glm::vec2)sprite.Sprite.GetTextureSize() / sprite.Sprite.GetSize());
            glm::i32vec2 lastSeperators = seperators;
            ImGui::DragInt2("Seperators", (int*)&seperators, 1.0f, 1, 0x7fffffff);
            if (seperators.x != lastSeperators.x || seperators.y != lastSeperators.y) {
                glm::vec2 texSize = sprite.Sprite.GetTextureSize();
                glm::vec2 size(texSize.x / seperators.x, texSize.y / seperators.y);
                sprite.Sprite.SetSubTexture(size, 0, 0);
            }
            glm::u32vec2 slots = { sprite.Sprite.GetSlotX(), sprite.Sprite.GetSlotY() };
            glm::u32vec2 lastSlots = slots;
            ImGui::DragInt2("Slot", (int*)&slots, 1.0f, 0, 0x7fffffff);
            if (slots.x != lastSlots.x || slots.y != lastSlots.y) {
                sprite.Sprite.SetSlot(slots.x, slots.y);
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

        if (entity.HasComponent<Nigozi::RigidbodyComponent>()) {
            if (ImGui::TreeNodeEx((void*)(typeid(Nigozi::RigidbodyComponent).hash_code() + (size_t)m_selectionContext),
                ImGuiTreeNodeFlags_DefaultOpen, "Rigidbody")) {
                auto& rigidbody = entity.GetComponent<Nigozi::RigidbodyComponent>();

                const char* bodyTypeNames[] = {
                    "Static", "Kinematic", "Dynamic"
                };
                int currentType = (int)rigidbody.Type;

                ImGui::Combo("Body Type", &currentType, bodyTypeNames, 3);
                rigidbody.Type = (Nigozi::RigidbodyComponent::BodyType)currentType;

                ImGui::Checkbox("Freeze Rotation", &rigidbody.FreezeRotation);

                ImGui::TreePop();
            }
        }

        if (entity.HasComponent<Nigozi::BoxColliderComponent>()) {
            if (ImGui::TreeNodeEx((void*)(typeid(Nigozi::BoxColliderComponent).hash_code() + (size_t)m_selectionContext),
                ImGuiTreeNodeFlags_DefaultOpen, "Box Collider")) {
                auto& boxCollider = entity.GetComponent<Nigozi::BoxColliderComponent>();

                ImGui::DragFloat2("Size", (float*)&boxCollider.Size, 0.05f, 0.01f);
                ImGui::DragFloat2("Offset", (float*)&boxCollider.Offset, 0.05f);

                ImGui::DragFloat("Density", &boxCollider.Density, 0.05f);
                ImGui::DragFloat("Friction", &boxCollider.Friction, 0.05f);
                ImGui::DragFloat("Restitution", &boxCollider.Restitution, 0.05f);
                ImGui::DragFloat("Restitution Threshold", &boxCollider.RestitutionThreshold, 0.05f);

                ImGui::TreePop();
            }
        }

        if (ImGui::TreeNodeEx((void*)(typeid(Nigozi::TransformComponent).hash_code() + (size_t)m_selectionContext),
            ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
            auto& transform = entity.GetComponent<Nigozi::TransformComponent>();

            ImGui::Text("Parent");
            ImGui::SameLine();
            Nigozi::UUID parentUUID = Nigozi::UUID::Null;
            Nigozi::Entity parent = entity.GetParent();
            if (entity.GetParent() != Nigozi::Entity()) {
                parentUUID = parent.GetUUID();
            }
            ImGui::Button(std::to_string(parentUUID).c_str());
            
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2{ 10.0f, 4.0f });
            
            ImGuiTableFlags flags = 
                ImGuiTableFlags_SizingFixedFit | 
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

            // --(1)SCALE TABLE END COLUMN
            ImGui::EndTable();

            // ----SCALE ROW
            ImGui::TableNextRow();

            // ----(0)SCALE_X COLUMN
            ImGui::TableNextColumn();
            ImGui::Text("Rotation");

            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-10.0f);
            ImGui::DragFloat("##ROTATION", &transform.Rotation, 0.1f, -360.0f, 360.0f, "%.2f", ImGuiSliderFlags_WrapAround);
            ImGui::PopItemWidth();

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

        enum class NodeTypes {
            NODE_2D,
            SPRITE_RENDERER,
            RIGID_BODY,
            AUDIO_STREAM_PLAYER,
            CAMERA
        };

        static std::array<std::string, 5> items = {
            "Node2D",
            "Sprite Renderer",
            "Rigidbody",
            "Audio Stream Player",
            "Camera"
        };
        static NodeTypes itemSelectedIndex = NodeTypes::NODE_2D;

        const char* defaultSelection = items[0].c_str();
        static char* preview;
        if (!preview) {
            preview = (char*)defaultSelection;
        }

        if (ImGui::BeginCombo("Nodes", preview, 0)) {
            for (size_t i = 0; i < items.size(); i++) {
                const bool isSelected = (itemSelectedIndex == (NodeTypes)i);
                if (ImGui::Selectable(items[i].c_str(), isSelected)) {
                    itemSelectedIndex = (NodeTypes)i;
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
            Nigozi::Entity entity = m_currentContext->CreateEntity("New Node", "Empty");

            if (entity.GetUUID() != m_currentContext->GetSceneRootUUID() && m_selectionContext != entt::null) {
                entity.SetParentUUID(Nigozi::Entity(m_selectionContext, m_currentContext.get()).GetUUID());
            }
            else if (entity.GetUUID() != m_currentContext->GetSceneRootUUID()) {
                entity.SetParentUUID(m_currentContext->GetSceneRootUUID());
            }

            AddComponent<Nigozi::SpriteRendererComponent>(entity, itemSelectedIndex == NodeTypes::SPRITE_RENDERER);
            AddComponent<Nigozi::RigidbodyComponent>(entity, itemSelectedIndex == NodeTypes::RIGID_BODY);
            // Rigidbody needs a box collider and we shoulnt have box collider as a seperate node
            AddComponent<Nigozi::BoxColliderComponent>(entity, itemSelectedIndex == NodeTypes::RIGID_BODY);
            AddComponent<Nigozi::AudioStreamPlayerComponent>(entity, itemSelectedIndex == NodeTypes::AUDIO_STREAM_PLAYER);
            AddComponent<Nigozi::CameraComponent>(entity, itemSelectedIndex == NodeTypes::CAMERA);

            if (itemSelectedIndex == NodeTypes::CAMERA) {
                auto cameraView = m_currentContext->m_Registry.view<Nigozi::CameraComponent>();
                for (auto [entityHandle, otherCamera] : cameraView.each()) {
                    if (entityHandle != entity.GetHandle()) {
                        otherCamera.Current = false;
                    }
                }
            }
            m_selectionContext = entity.GetHandle();

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
    if (ImGui::Button("Play")) {
        m_editorState = EditorState::PLAY;
        m_currentContext->OnAttach();
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause") && m_editorState == EditorState::PLAY) {
        m_editorState = EditorState::PAUSE;
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) {
        Nigozi::Entity entity = m_currentContext->TryGetEntityByUUID(m_currentContext->GetSceneRootUUID());
        auto scene = entity.GetComponent<Nigozi::SceneComponent>();
        if (std::filesystem::exists(scene.FilePath) && !std::filesystem::is_directory(scene.FilePath)) {
            m_currentContext->OnDetach();
            m_currentContext->ClearSceneTree();
            m_currentContext->DeserializeScene(scene.FilePath);
            m_selectionContext = entt::null;
            m_movingSelectionContext = entt::null;
        }
        m_editorState = EditorState::EDIT;
    }
    if (ImGui::IsItemHovered()) buttonHovered = true;

    m_viewportSize = ImGui::GetContentRegionAvail();

    ImVec2 viewportFrameSize = ImGui::GetContentRegionMax();
    if (m_editorState == EditorState::PLAY || m_editorState == EditorState::PAUSE) {
        auto cameraView = m_currentContext->m_Registry.view<Nigozi::CameraComponent>();
        for (auto [entity, camera] : cameraView.each()) {
            if (!camera.Current) {
                continue;
            }
            float availableAspect = m_viewportSize.x / m_viewportSize.y;
            if (camera.Aspect > availableAspect) {
                m_viewportSize.y = m_viewportSize.x / camera.Aspect;
            }
            else {
                m_viewportSize.x = m_viewportSize.y / (1.0f / camera.Aspect);
            }
            break;
        }
    }
    ImVec2 padding{ viewportFrameSize.x - m_viewportSize.x, viewportFrameSize.y - m_viewportSize.y };
    padding.y += ImGui::GetFrameHeight();

    ImVec2 availableFrameSize = ImGui::GetContentRegionAvail();
    ImVec2 offset
    { 
        (availableFrameSize.x - m_viewportSize.x) / 2.0f,
        (availableFrameSize.y - m_viewportSize.y) / 2.0f 
    };

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset.x);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset.y);

    m_viewportPosition = glm::vec2(ImGui::GetWindowPos().x + padding.x + offset.x, ImGui::GetWindowPos().y + padding.y + offset.y);

    ImGui::Image((uint64_t)(p_viewportBuffer->GetColorAttachment()), m_viewportSize, ImVec2(0, 1), ImVec2(1, 0));
    m_viewportHovered = ImGui::IsWindowHovered() && !buttonHovered;
    ImGui::End();
}
