#include "EditorLayer.h"
#include "project/ProjectUtils.h"

bool EditorLayer::s_showDemoWindow = false;

EditorLayer::EditorLayer(const EditorParams& params, Nigozi::FrameBuffer* viewportBuffer)
    :m_mouseOldPosition(0.0f), m_viewportPosition(0.0f), m_windowPosition(0.0f),
    m_editorCamera(viewportBuffer->GetWidth() / (float)viewportBuffer->GetHeight(), 5.0f),
    m_editorParams(params)
{
    m_currentContext = std::make_shared<Nigozi::SceneTree>();
    m_sceneTreeContexts.push_back(m_currentContext);

    p_viewportBuffer = viewportBuffer;
}

void EditorLayer::OnAttach()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.FontDefault = io.Fonts->AddFontFromFileTTF(std::filesystem::path("src/res/fonts/Open_Sans/static/OpenSans-Medium.ttf").string().c_str(), 18.0f);
    
    ImGuiStyle& style = ImGui::GetStyle();

    // ---- Shape ------------------------------------------------------------------
    style.WindowRounding = 4.f;
    style.ChildRounding = 4.f;
    style.FrameRounding = 4.f;
    style.PopupRounding = 4.f;
    style.ScrollbarRounding = 6.f;
    style.GrabRounding = 4.f;
    style.TabRounding = 4.f;

    style.WindowBorderSize = 1.f;
    style.FrameBorderSize = 0.f;
    style.PopupBorderSize = 1.f;

    style.WindowPadding = { 12.f, 10.f };
    style.FramePadding = { 8.f,  6.f };
    style.ItemSpacing = { 8.f,  6.f };
    style.ItemInnerSpacing = { 6.f,  5.f };
    style.IndentSpacing = 18.f;
    style.ScrollbarSize = 8.f;
    style.GrabMinSize = 8.f;

    // Button centred so icons sit in the middle of their cell
    style.ButtonTextAlign = { 0.5f, 0.5f };
    style.SelectableTextAlign = { 0.0f, 0.5f };

    auto color = [](int red, int green, int blue, int alpha = 255) -> ImVec4 {
        return { red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f };
        };

    const ImVec4 bg_deep = color(15, 15, 18);
    const ImVec4 bg_base = color(22, 22, 26);   // window / panel
    const ImVec4 bg_raised = color(30, 30, 35);   // frames, inputs
    const ImVec4 bg_pop = color(38, 38, 44);   // buttons, popups
    const ImVec4 bg_hover = color(47, 47, 53);   // hover
    const ImVec4 bg_active = color(56, 56, 63);   // pressed (non-accent)
    const ImVec4 border = color(35, 35, 41);   // near-invisible borders
    const ImVec4 text = color(224, 224, 232);   // primary text (cooler white)
    const ImVec4 text_dim = color(82, 82, 92);   // disabled / secondary
    const ImVec4 accent = color(49, 197, 209);
    const ImVec4 accent_d = color(49, 197, 209, 180);
    const ImVec4 accent_bg = color(49, 197, 209, 35);

    ImVec4* col = ImGui::GetStyle().Colors;

    col[ImGuiCol_Text] = text;
    col[ImGuiCol_TextDisabled] = text_dim;
    col[ImGuiCol_WindowBg] = bg_base;
    col[ImGuiCol_ChildBg] = bg_base;
    col[ImGuiCol_PopupBg] = color(26, 26, 30);
    col[ImGuiCol_Border] = border;
    col[ImGuiCol_BorderShadow] = color(0, 0, 0, 0);
    col[ImGuiCol_FrameBg] = bg_raised;
    col[ImGuiCol_FrameBgHovered] = bg_hover;
    col[ImGuiCol_FrameBgActive] = bg_active;
    col[ImGuiCol_TitleBg] = bg_deep;
    col[ImGuiCol_TitleBgActive] = bg_deep;
    col[ImGuiCol_TitleBgCollapsed] = bg_deep;
    col[ImGuiCol_MenuBarBg] = bg_deep;
    col[ImGuiCol_ScrollbarBg] = color(0, 0, 0, 0);
    col[ImGuiCol_ScrollbarGrab] = color(60, 60, 80, 140);
    col[ImGuiCol_ScrollbarGrabHovered] = color(80, 80, 100, 180);
    col[ImGuiCol_ScrollbarGrabActive] = accent;
    col[ImGuiCol_CheckMark] = accent;
    col[ImGuiCol_SliderGrab] = accent_d;
    col[ImGuiCol_SliderGrabActive] = accent;
    col[ImGuiCol_Button] = bg_pop;
    col[ImGuiCol_ButtonHovered] = bg_hover;
    col[ImGuiCol_ButtonActive] = accent;
    col[ImGuiCol_Header] = accent_bg;
    col[ImGuiCol_HeaderHovered] = color(49, 197, 209, 60);
    col[ImGuiCol_HeaderActive] = accent_d;
    col[ImGuiCol_Separator] = border;
    col[ImGuiCol_SeparatorHovered] = accent_d;
    col[ImGuiCol_SeparatorActive] = accent;
    col[ImGuiCol_ResizeGrip] = color(0, 0, 0, 0);
    col[ImGuiCol_ResizeGripHovered] = accent_d;
    col[ImGuiCol_ResizeGripActive] = accent;
    col[ImGuiCol_Tab] = bg_raised;
    col[ImGuiCol_TabHovered] = bg_hover;
    col[ImGuiCol_TabSelected] = bg_pop;
    col[ImGuiCol_TabSelectedOverline] = accent;
    col[ImGuiCol_TabDimmed] = bg_base;
    col[ImGuiCol_TabDimmedSelected] = bg_raised;
    col[ImGuiCol_TabDimmedSelectedOverline] = border;
    col[ImGuiCol_DockingPreview] = accent_d;
    col[ImGuiCol_DockingEmptyBg] = bg_deep;
    col[ImGuiCol_PlotLines] = accent;
    col[ImGuiCol_PlotLinesHovered] = color(255, 200, 80);
    col[ImGuiCol_PlotHistogram] = accent;
    col[ImGuiCol_PlotHistogramHovered] = color(255, 200, 80);
    col[ImGuiCol_TableHeaderBg] = bg_deep;
    col[ImGuiCol_TableBorderStrong] = border;
    col[ImGuiCol_TableBorderLight] = color(30, 30, 35);
    col[ImGuiCol_TableRowBg] = color(0, 0, 0, 0);
    col[ImGuiCol_TableRowBgAlt] = color(255, 255, 255, 5);
    col[ImGuiCol_TextSelectedBg] = accent_bg;
    col[ImGuiCol_DragDropTarget] = accent;
    col[ImGuiCol_NavHighlight] = accent;
    col[ImGuiCol_NavWindowingHighlight] = accent;
    col[ImGuiCol_NavWindowingDimBg] = color(0, 0, 0, 100);
    col[ImGuiCol_ModalWindowDimBg] = color(0, 0, 0, 140);
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
        Hotkeys();
        return;
    }
    if (m_editorState == EditorState::PAUSE) {
        return;
    }
    m_currentContext->OnUpdate(timestep);
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

    auto boxColliderView = m_currentContext->m_Registry.view<Nigozi::TransformComponent, Nigozi::BoxColliderComponent>();
    for (auto [entityHandle, transform, boxCollider] : boxColliderView.each()) {
        auto worldTransform = m_currentContext->GetWorldSpaceTransform(Nigozi::Entity(entityHandle, m_currentContext.get()));
        glm::vec4 color(0.0f, 0.7f, 0.1f, 1.0f);
        if (m_selectionContext == entityHandle) {
            color = glm::vec4(0.0f, 1.0f, 0.2f, 1.0f);
        }
        Nigozi::Renderer2D::DrawRotatedQuad(
            worldTransform.Position,
            glm::vec2(boxCollider.Size.x * 2.0f, boxCollider.Size.y * 2.0f),
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
    ShowFileTreeExplorer();
    ShowInspector();
    ShowViewportPanel();
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
    ShowCreateOrOpenProjectModal();
}

bool EditorLayer::OnSelectMouseButtonPressed(Nigozi::MouseButtonPressedEvent& event)
{
    if (m_tool != Tool::SELECT || !m_viewportHovered) {
        return false;
    }
    if (event.GetButton() == GLFW_MOUSE_BUTTON_1) {
        glm::vec2 viewportRelPos = m_viewportPosition - m_windowPosition;

        Nigozi::OrthographicCamera camera = m_editorCamera.GetCamera();
        if (m_editorState != EditorState::EDIT) {
            Nigozi::CameraComponent* cameraComponent = m_currentContext->GetMainCamera();
            if (cameraComponent) {
                float aspect = cameraComponent->Aspect;
                float zoom = cameraComponent->Zoom;
                camera = Nigozi::OrthographicCamera(-aspect * zoom, aspect * zoom, -zoom, zoom);
            }
        }

        glm::vec2 mousePosition = camera.GetMousePositionWorldSpace(Nigozi::Input::GetMousePosition() - viewportRelPos, *(glm::vec2*)&m_viewportSize);

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

void EditorLayer::Hotkeys()
{
    // Undo - CTRL+Z
    if (Nigozi::Input::IsKeyPressed(NG_KEY_LEFT_CONTROL) &&
        Nigozi::Input::IsKeyJustPressed(NG_KEY_Z)) {
        m_commandQueue.RevertBack();
        return;
    }
    // Redo - CTRL+SHIFT+Z CTRL+Y
    if ((Nigozi::Input::IsKeyPressed(NG_KEY_LEFT_CONTROL) &&
        Nigozi::Input::IsKeyPressed(NG_KEY_LEFT_SHIFT) &&
        Nigozi::Input::IsKeyJustPressed(NG_KEY_Z)) ||
        (Nigozi::Input::IsKeyPressed(NG_KEY_LEFT_CONTROL) &&
        Nigozi::Input::IsKeyJustPressed(NG_KEY_Y))) {
        m_commandQueue.RedoBack();
        return;
    }
    // Save - CTRL+S
    if (Nigozi::Input::IsKeyPressed(NG_KEY_LEFT_CONTROL) &&
        Nigozi::Input::IsKeyJustPressed(NG_KEY_S)) {
        SaveCurrentScene();
        return;
    }
    // Save As - CTRL+SHIFT+S
    if (Nigozi::Input::IsKeyPressed(NG_KEY_LEFT_CONTROL) &&
        Nigozi::Input::IsKeyPressed(NG_KEY_LEFT_SHIFT) &&
        Nigozi::Input::IsKeyJustPressed(NG_KEY_S)) {
        SaveCurrentSceneAs();
        return;
    }
    // Save All - CTRL+SHIFT+S
    if (Nigozi::Input::IsKeyPressed(NG_KEY_LEFT_CONTROL) &&
        Nigozi::Input::IsKeyPressed(NG_KEY_LEFT_ALT) &&
        Nigozi::Input::IsKeyPressed(NG_KEY_LEFT_SHIFT) &&
        Nigozi::Input::IsKeyJustPressed(NG_KEY_S)) {
        SaveCurrentSceneAs();
        return;
    }
}

bool EditorLayer::OnMouseMoved(Nigozi::MouseMovedEvent& event)
{
    if (m_tool != Tool::SELECT && m_tool != Tool::MOVE || m_movingSelectionContext == entt::null) {
        m_mouseOldPosition = glm::vec2(event.GetX(), event.GetY());
        return false;
    }
    auto entity = Nigozi::Entity(m_movingSelectionContext, m_currentContext.get());
    auto& transform = entity.GetComponent<Nigozi::TransformComponent>();
    auto worldTransform = m_currentContext->GetWorldSpaceTransform(Nigozi::Entity(m_movingSelectionContext, m_currentContext.get()));

    glm::vec2 viewportRelPos = m_viewportPosition - m_windowPosition;
    glm::vec2 mouseScreenPosition = glm::vec2(event.GetX(), event.GetY());
    
    Nigozi::OrthographicCamera camera = m_editorCamera.GetCamera();
    if (m_editorState != EditorState::EDIT) {
        Nigozi::CameraComponent* cameraComponent = m_currentContext->GetMainCamera();
        if (cameraComponent) {
            float aspect = cameraComponent->Aspect;
            float zoom = cameraComponent->Zoom;
            camera = Nigozi::OrthographicCamera(-aspect * zoom, aspect * zoom, -zoom, zoom);
        }
    }
    
    glm::vec2 mousePosition = camera.GetMousePositionWorldSpace(glm::vec2(event.GetX(), event.GetY()) - viewportRelPos, *(glm::vec2*)&m_viewportSize);

    glm::vec2 mouseOldPosition = camera.GetMousePositionWorldSpace(m_mouseOldPosition - viewportRelPos, *(glm::vec2*)&m_viewportSize);

    glm::vec2 mouseDelta = mousePosition - mouseOldPosition;

    // Transform the point into rectangle space
    float rotation = glm::radians(-(worldTransform.Rotation - transform.Rotation));
    glm::vec2 normal(glm::cos(rotation), glm::sin(-rotation));
    glm::vec2 rotated(mouseDelta.x * normal.x - mouseDelta.y * normal.y,
        mouseDelta.x * normal.y + mouseDelta.y * normal.x);

    transform.Position += rotated;

    m_mouseOldPosition = mouseScreenPosition;

    if (m_editorState != EditorState::EDIT) {
        if (!entity.HasComponent<Nigozi::RigidbodyComponent>()) {
            return false;
        }
        auto& rigidbody = entity.GetComponent<Nigozi::RigidbodyComponent>();
        b2Body* body = (b2Body*)rigidbody.RuntimeBody;
        auto rTransform = body->GetTransform();
        rTransform.p.x += rotated.x;
        rTransform.p.y += rotated.y;
        body->SetTransform(rTransform.p, rTransform.q.GetAngle());
        body->SetAwake(true);
    }
    return false;
}

bool EditorLayer::OnRotateMouseMoved(Nigozi::MouseMovedEvent& event)
{
    if (m_tool != Tool::ROTATE || m_movingSelectionContext == entt::null) {
        return false;
    }
    auto entity = Nigozi::Entity(m_movingSelectionContext, m_currentContext.get());
    auto& transform = entity.GetComponent<Nigozi::TransformComponent>();
    auto worldTransform = m_currentContext->GetWorldSpaceTransform(Nigozi::Entity(m_movingSelectionContext, m_currentContext.get()));

    glm::vec2 viewportRelPos = m_viewportPosition - m_windowPosition;
    glm::vec2 mouseScreenPosition = glm::vec2(event.GetX(), event.GetY());

    Nigozi::OrthographicCamera camera = m_editorCamera.GetCamera();
    if (m_editorState != EditorState::EDIT) {
        Nigozi::CameraComponent* cameraComponent = m_currentContext->GetMainCamera();
        if (cameraComponent) {
            float aspect = cameraComponent->Aspect;
            float zoom = cameraComponent->Zoom;
            camera = Nigozi::OrthographicCamera(-aspect * zoom, aspect * zoom, -zoom, zoom);
        }
    }

    glm::vec2 mousePosition = camera.GetMousePositionWorldSpace(glm::vec2(event.GetX(), event.GetY()) - viewportRelPos, *(glm::vec2*)&m_viewportSize);

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
    if (m_editorState != EditorState::EDIT) {
        if (!entity.HasComponent<Nigozi::RigidbodyComponent>()) {
            return false;
        }
        auto& rigidbody = entity.GetComponent<Nigozi::RigidbodyComponent>();
        b2Body* body = (b2Body*)rigidbody.RuntimeBody;
        auto rTransform = body->GetTransform();
        body->SetTransform(rTransform.p, glm::radians(angle));
        body->SetAwake(true);
    }

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
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                SaveCurrentScene();
                Nigozi::ScriptEngine::ReloadAssemblies();
            }
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
                SaveCurrentSceneAs();
            }
            if (ImGui::MenuItem("Save All Scenes...", "Ctrl+Alt+Shift+S")) {
                SaveAllScenes();
            }
            if (ImGui::MenuItem("Load")) {
                LoadScene();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                m_commandQueue.RevertBack();
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Shift+Z")) {
                m_commandQueue.RedoBack();
            }
            ImGui::Separator();
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
        | ((entity.GetChildrenUUIDs().size() == 0) ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)
        | ImGuiTreeNodeFlags_SpanAvailWidth;

    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.GetHandle(), flags, nameComponent.Name.c_str());

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        if (m_selectionContext == entity.GetHandle()) {
            m_selectionContext = entt::null;
        }
        else {
            m_selectionContext = entity.GetHandle();
        }
    }

    if (ImGui::IsItemHovered() && ImGui::BeginDragDropTarget()) {
        Nigozi::UUID dragChildUUID = *(uint64_t*)ImGui::GetDragDropPayload()->Data;
        if (dragChildUUID != entity.GetUUID()) {
            Nigozi::Entity child = m_currentContext->TryGetEntityByUUID(dragChildUUID);
            child.SetParentUUID(entity.GetUUID());
        }
        ImGui::EndDragDropTarget();
    }

    // don't allow drag dropping root node
    if (ImGui::BeginDragDropSource() && entity.GetParent() != Nigozi::Entity()) {
        uint64_t uuidHandle = entity.GetUUID();
        ImGui::SetDragDropPayload("SCENE_HIERARCHY_NODE_DRAG_DROP", &uuidHandle, sizeof(uint64_t), ImGuiCond_Once);
        ImGui::EndDragDropSource();
    }

    if (opened) {
        for (Nigozi::Entity child : entity.GetChildren()) {
            DrawSceneHierarchyNode(child);
        }
        ImGui::TreePop();
    }
}

void EditorLayer::DrawFileTreeDirectory(std::filesystem::path& dirPath)
{
    std::array<std::string, 1> ignoreExtensions{
        ".csproj"
    };
    std::array<std::string, 1> ignoreFiles{
        "premake5.lua"
    };
    std::array<std::string, 1> ignoreDirectories{
        "obj"
    };

    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        std::filesystem::path entryPath = entry.path();
        std::filesystem::path entryPathRelative = entryPath.lexically_relative(dirPath);

        std::string extension = entryPath.extension().string();
        std::string filename = entryPath.filename().string();

        bool ignored = false;
        for (const auto& ignore : ignoreExtensions) {
            if (extension == ignore) {
                ignored = true;
                break;
            }
        }
        for (const auto& ignore : ignoreFiles) {
            if (filename == ignore) {
                ignored = true;
                break;
            }
        }
        for (const auto& ignore : ignoreDirectories) {
            if (filename == ignore) {
                ignored = true;
                break;
            }
        }
        if (ignored) {
            continue;
        }

        bool isDirectory = std::filesystem::is_directory(entryPath);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
        if (!isDirectory) {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }
        if (ImGui::TreeNodeEx(entryPath.string().c_str(), flags, entryPathRelative.filename().string().c_str())) {
            if (isDirectory) {
                if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                    ImGui::OpenPopup("##FileTreeDirectoryPopup");
                }
                if (ImGui::BeginPopup("##FileTreeDirectoryPopup")) {
                    constexpr int CREATE_DIR_ID = 0;
                    constexpr int DELETE_DIR_ID = 1;
                    std::array<const char*, 2> names = { 
                        "Create Directory",
                        "Delete Directory"
                    };
                    static int selectedDirOperation = 0;
                    for (size_t i = 0; i < names.size(); i++) {
                        if (ImGui::Selectable(names[i])) {
                            selectedDirOperation = i;
                        }
                    }
                    if (selectedDirOperation == CREATE_DIR_ID) {
                        
                    }
                    else if (selectedDirOperation == DELETE_DIR_ID) {

                    }
                    ImGui::EndPopup();
                }
                DrawFileTreeDirectory(entryPath);
            }
            else {
                // This drag drop will be targeting veiwport
                // To find that snippet of code, CTRL + F
                // VIEWPORT_FILE_TREE_ITEM_DRAG_DROP
                if (ImGui::BeginDragDropSource()) {
                    std::string entryPathString = entryPath.string();
                    ImGui::SetDragDropPayload("FILE_TREE_ITEM", entryPathString.data(), (entryPathString.size() + 1), ImGuiCond_Once);
                    m_isDragDropping = true;
                    ImGui::EndDragDropSource();
                }
                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    if (entryPath.extension() == ".ngscn") {
                        LoadScene(entryPath);
                    }
                }
            }
            ImGui::TreePop();
        }
    }
}

void EditorLayer::ShowFileTreeExplorer()
{
    ImGui::Begin("File Explorer");
    if (Nigozi::Project::s_ProjectDir.empty() || !std::filesystem::exists(Nigozi::Project::s_ProjectDir / "Assets")) {
        ImGui::End();
        return;
    }
    std::filesystem::path projectPath = Nigozi::Project::s_ProjectDir / "Assets";
    DrawFileTreeDirectory(projectPath);
    ImGui::End();
}

template<typename T>
static void DrawComponentInInspector(const std::string& name, Nigozi::Entity entity, entt::entity selectionContext, std::function<void(T&)> func) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

    if (entity.HasComponent<T>() &&
        ImGui::TreeNodeEx((void*)(typeid(T).hash_code() + (size_t)selectionContext), flags, name.c_str()))
    {
        auto& component = entity.GetComponent<T>();

        func(component);

        ImGui::TreePop();
        ImGui::Separator();
    }
}

void EditorLayer::UpdateRigidbodyTransform(Nigozi::Entity entity) {
    if (m_editorState == EditorState::EDIT) {
        return;
    }
    if (!entity.HasComponent<Nigozi::RigidbodyComponent>()) {
        return;
    }
    auto& transform = entity.GetComponent<Nigozi::TransformComponent>();
    auto& rigidbody = entity.GetComponent<Nigozi::RigidbodyComponent>();
    b2Body* body = (b2Body*)rigidbody.RuntimeBody;
    auto rTransform = body->GetTransform();
    auto worldTransform = m_currentContext->GetWorldSpaceTransform(entity);
    rTransform.p.x = worldTransform.Position.x;
    rTransform.p.y = worldTransform.Position.y;
    body->SetTransform(rTransform.p, glm::radians(-worldTransform.Rotation));
    body->SetAwake(true);
}

void EditorLayer::UpdateBoxCollider(Nigozi::Entity entity, glm::vec2 size)
{
    if (m_editorState == EditorState::EDIT) {
        return;
    }
    auto& boxCollider = entity.GetComponent<Nigozi::BoxColliderComponent>();
    b2Fixture* oldFixture = (b2Fixture*)boxCollider.RuntimeFixture;

    auto& rigidbody = entity.GetComponent<Nigozi::RigidbodyComponent>();
    b2Body* body = (b2Body*)rigidbody.RuntimeBody;
    
    float density = oldFixture->GetDensity();
    float friction = oldFixture->GetFriction();
    float restitution = oldFixture->GetRestitution();
    bool isSensor = oldFixture->IsSensor();

    body->DestroyFixture(oldFixture);

    b2PolygonShape newShape;
    newShape.SetAsBox(size.x, size.y);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &newShape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;

    boxCollider.RuntimeFixture = (void*)body->CreateFixture(&fixtureDef);

    body->ResetMassData();
}

void EditorLayer::ShowInspector()
{
    ImGui::Begin("Inspector");
    if (m_selectionContext == entt::null) {
        ImGui::End();
        return;
    }
    Nigozi::Entity entity(m_selectionContext, m_currentContext.get());

    auto& uuidComponent = entity.GetComponent<Nigozi::UUIDComponent>();
    auto& nameComponent = entity.GetComponent<Nigozi::NameComponent>();
    auto& tagComponent = entity.GetComponent<Nigozi::TagComponent>();
    ImGui::Text(std::to_string(uuidComponent.ID.GetUUID()).c_str());
    ImGui::InputText("Name", nameComponent.Name.data(), nameComponent.Name.capacity());
    ImGui::InputText("Tag", tagComponent.Tag.data(), tagComponent.Tag.capacity());

    if (!entity.HasComponent<Nigozi::ScriptComponent>()) {
        if (ImGui::Button("Add Script +")) {
            entity.AddComponent<Nigozi::ScriptComponent>();
        }
    }
    DrawComponentInInspector<Nigozi::ScriptComponent>("Script", entity, m_selectionContext,
        [&](auto& component) {
            auto name = component.ScriptPath.string();
            if (name.empty()) {
                name = "***";
            }
            if (ImGui::Button(name.c_str())) {
                std::filesystem::path newPath = Nigozi::FileDialogue::OpenFileDialog("cs");
                if (!newPath.empty()) {
                    component.ScriptPath = newPath;
                }
            }
        });
    DrawComponentInInspector<Nigozi::CameraComponent>("Camera", entity, m_selectionContext,
        [&](auto& component) {
            ImGui::DragFloat("Zoom", &component.Zoom, 0.05f, 0.01f);
            if (ImGui::Checkbox("Current", &component.Current)) {
                auto cameraView = m_currentContext->m_Registry.view<Nigozi::CameraComponent>();
                for (auto [entityHandle, otherCamera] : cameraView.each()) {
                    if (entityHandle == entity.GetHandle()) {
                        continue;
                    }
                    otherCamera.Current = false;
                }
            }
            ImGui::DragFloat("Aspect", &component.Aspect, 0.1f, 0.01f);
        }
    );
    DrawComponentInInspector<Nigozi::SpriteRendererComponent>("Sprite Renderer", entity, m_selectionContext,
        [&](auto& component) {
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
            if (ImGui::ImageButton("Sprite", component.SpriteTexture->GetRendererID(), size, uv0, uv1, bg_col, tint_col)) {
                std::filesystem::path result = Nigozi::FileDialogue::OpenFileDialog("png;jpg;jpeg");
                if (!result.empty()) {
                    std::string path = result.string();

                    component.SpriteTexture.reset();
                    component.SpriteTexture = std::make_shared<Nigozi::Texture>(path);
                    component.Sprite = Nigozi::SubTexture(component.SpriteTexture, component.SpriteTexture->GetSize());
                }
            }

            EditValueInInspector<glm::vec4>(
                [&]() { ImGui::ColorEdit4("Color", (float*)&component.Color); },
                [&](glm::vec4 color) {
                    m_commandQueue.PushBack(Command(
                        [&, newColor = component.Color, uuid = uuidComponent.ID](void* data) {
                            auto& sprite = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::SpriteRendererComponent>();
                            sprite.Color = newColor;
                        },
                        [&, oldColor = color, uuid = uuidComponent.ID](void* data) {
                            auto& sprite = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::SpriteRendererComponent>();
                            sprite.Color = oldColor;
                        }
                    ));
                },
                uuidComponent.ID,
                component.Color
            );
            int zorder = component.ZOrder;
            EditValueInInspector<int>(
                [&]() { ImGui::SliderInt("Z Order", &zorder, -127, 127); component.ZOrder = (int16_t)zorder; },
                [&](int zorder) {
                    m_commandQueue.PushBack(Command(
                        [&, newZOrder = component.ZOrder, uuid = uuidComponent.ID](void* data) {
                            auto& sprite = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::SpriteRendererComponent>();
                            sprite.ZOrder = (int16_t)newZOrder;
                        },
                        [&, oldZOrder = zorder, uuid = uuidComponent.ID](void* data) {
                            auto& sprite = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::SpriteRendererComponent>();
                            sprite.ZOrder = (int16_t)oldZOrder;
                        }
                    ));
                },
                uuidComponent.ID,
                component.ZOrder
            );

            glm::i32vec2 seperators = glm::i32vec2((glm::vec2)component.Sprite.GetTextureSize() / component.Sprite.GetSize());
            glm::i32vec2 lastSeperators = seperators;
            ImGui::DragInt2("Seperators", (int*)&seperators, 1.0f, 1, 0x7fffffff);
            if (seperators.x != lastSeperators.x || seperators.y != lastSeperators.y) {
                glm::vec2 texSize = component.Sprite.GetTextureSize();
                glm::vec2 size(texSize.x / seperators.x, texSize.y / seperators.y);
                component.Sprite.SetSubTexture(size, 0, 0);
            }
            glm::u32vec2 slots = { component.Sprite.GetSlotX(), component.Sprite.GetSlotY() };
            glm::u32vec2 lastSlots = slots;
            ImGui::DragInt2("Slot", (int*)&slots, 1.0f, 0, 0x7fffffff);
            if (slots.x != lastSlots.x || slots.y != lastSlots.y) {
                component.Sprite.SetSlot(slots.x, slots.y);
            }
        }
    );
    DrawComponentInInspector<Nigozi::AudioStreamPlayerComponent>("Audio Stream Player", entity, m_selectionContext,
        [&](auto& component) {
            if (ImGui::Button("Choose")) {
                std::filesystem::path result = Nigozi::FileDialogue::OpenFileDialog("wav,mp3,flac");
                NG_CORE_LOG_INFO("New audio path: {}", result.string());
                if (!result.empty()) {
                    Nigozi::Audio* newAudio = nullptr;
                    if (component.AudioHandle) {
                        newAudio = Nigozi::AudioEngine::LoadAudioFromFile(result, component.AudioHandle->GetAudioGroupName());
                    }
                    else {
                        newAudio = Nigozi::AudioEngine::LoadAudioFromFile(result);
                    }
                    NG_CORE_LOG_INFO("New audio: {}", (size_t)newAudio);
                    if (newAudio) {
                        if (component.AudioHandle) {
                            Nigozi::AudioEngine::UnloadAudio(component.AudioHandle);
                        }
                        component.AudioHandle = newAudio;
                        component.AudioHandle->SetVolume(component.Volume);
                        NG_CORE_LOG_INFO("Set audio: {}", (size_t)component.AudioHandle);
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
            if (component.AudioHandle) {
                audioPath = component.AudioHandle->GetFilePath();
            }
            std::string audioPathString = audioPath.string();
            if (audioPathString.size() > 32) {
                audioPathString = audioPathString.substr(audioPathString.size() - 32);
            }
            ImGui::Text(audioPathString.c_str());
            if (component.AudioHandle) {
                EditValueInInspector<float>(
                    [&]() {
                        if (ImGui::DragFloat("Volume", &component.Volume, 0.2f)) {
                            component.AudioHandle->SetVolume(component.Volume);
                        }
                    },
                    [&](float volume) {
                        m_commandQueue.PushBack(Command(
                            [&, newVolume = component.Volume, uuid = uuidComponent.ID](void* data) {
                                auto& audio = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::AudioStreamPlayerComponent>();
                                audio.Volume = newVolume;
                                audio.AudioHandle->SetVolume(audio.Volume);
                            },
                            [&, oldVolume = volume, uuid = uuidComponent.ID](void* data) {
                                auto& audio = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::AudioStreamPlayerComponent>();
                                audio.Volume = oldVolume;
                                audio.AudioHandle->SetVolume(audio.Volume);
                            }
                        ));
                    },
                    uuidComponent.ID,
                    component.Volume
                );
                bool lastPlayingVal = component.AudioHandle->IsPlaying();
                bool playingVal = lastPlayingVal;
                EditValueInInspector<bool>(
                    [&]() {
                        if (ImGui::Checkbox("Playing", &playingVal)) {
                            if (playingVal != lastPlayingVal) {
                                if (playingVal) {
                                    component.AudioHandle->Play();
                                }
                                else {
                                    component.AudioHandle->Stop();
                                }
                            }
                        }
                    },
                    [&](bool isPlaying) {
                        m_commandQueue.PushBack(Command(
                            [&, newPlaying = component.AudioHandle->IsPlaying(), uuid = uuidComponent.ID](void* data) {
                                auto& audio = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::AudioStreamPlayerComponent>();
                                if (newPlaying)
                                    audio.AudioHandle->Play();
                                else
                                    audio.AudioHandle->Stop();
                            },
                            [&, oldPlaying = isPlaying, uuid = uuidComponent.ID](void* data) {
                                auto& audio = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::AudioStreamPlayerComponent>();
                                if (oldPlaying)
                                    audio.AudioHandle->Play();
                                else
                                    audio.AudioHandle->Stop();
                            }
                        ));
                    },
                    uuidComponent.ID,
                    playingVal
                );
            }
        }
    );
    DrawComponentInInspector<Nigozi::RigidbodyComponent>("Rigidbody", entity, m_selectionContext,
        [&](auto& rigidbody) {
            auto& component = entity.GetComponent<Nigozi::RigidbodyComponent>();

            const char* bodyTypeNames[] = {
                "Static", "Kinematic", "Dynamic"
            };
            int currentType = (int)component.Type;
            EditValueInInspector<Nigozi::RigidbodyComponent::BodyType>(
                [&]() { ImGui::Combo("Body Type", &currentType, bodyTypeNames, 3); component.Type = (Nigozi::RigidbodyComponent::BodyType)currentType; },
                [&](Nigozi::RigidbodyComponent::BodyType type) {
                    m_commandQueue.PushBack(Command(
                        [&, newType = component.Type, uuid = uuidComponent.ID](void* data) {
                            auto& rigidbody = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::RigidbodyComponent>();
                            rigidbody.Type = newType;
                        },
                        [&, oldType = type, uuid = uuidComponent.ID](void* data) {
                            auto& rigidbody = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::RigidbodyComponent>();
                            rigidbody.Type = oldType;
                        }
                    ));
                },
                uuidComponent.ID,
                component.Type,
                EditValueInInspectorFlags::COMBO_BOX
            );

            EditValueInInspector<bool>(
                [&]() { ImGui::Checkbox("Freeze Rotation", &component.FreezeRotation); },
                [&](bool freezeRotation) {
                    m_commandQueue.PushBack(Command(
                        [&, newRotation = component.FreezeRotation, uuid = uuidComponent.ID](void* data) {
                            auto& rigidbody = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::RigidbodyComponent>();
                            rigidbody.FreezeRotation = newRotation;
                        },
                        [&, oldRotation = freezeRotation, uuid = uuidComponent.ID](void* data) {
                            auto& rigidbody = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::RigidbodyComponent>();
                            rigidbody.FreezeRotation = oldRotation;
                        }
                    ));
                },
                uuidComponent.ID,
                component.FreezeRotation
            );
        }
    );
    DrawComponentInInspector<Nigozi::BoxColliderComponent>("Box Collider", entity, m_selectionContext,
        [&](auto& component) {
            EditValueInInspector<glm::vec2>(
                [&]() 
                { 
                    if (ImGui::DragFloat2("Size", (float*)&component.Size, 0.05f, 0.01f)) {
                        UpdateBoxCollider(entity, component.Size);
                    }
                },
                [&](glm::vec2 size) {
                    m_commandQueue.PushBack(Command(
                        [&, newSize = component.Size, uuid = uuidComponent.ID](void* data) {
                            auto& boxCollider = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::BoxColliderComponent>();
                            UpdateBoxCollider(entity, newSize);
                            boxCollider.Size = newSize;
                        },
                        [&, oldSize = size, uuid = uuidComponent.ID](void* data) {
                            auto& boxCollider = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::BoxColliderComponent>();
                            UpdateBoxCollider(entity, oldSize);
                            boxCollider.Size = oldSize;
                        }
                    ));
                },
                uuidComponent.ID,
                component.Size
            );
            EditValueInInspector<glm::vec2>(
                [&]() { ImGui::DragFloat2("Offset", (float*)&component.Offset, 0.05f); },
                [&](glm::vec2 offset) {
                    m_commandQueue.PushBack(Command(
                        [&, newOffset = component.Offset, uuid = uuidComponent.ID](void* data) {
                            auto& boxCollider = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::BoxColliderComponent>();
                            boxCollider.Offset = newOffset;
                        },
                        [&, oldOffset = offset, uuid = uuidComponent.ID](void* data) {
                            auto& boxCollider = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::BoxColliderComponent>();
                            boxCollider.Offset = oldOffset;
                        }
                    ));
                },
                uuidComponent.ID,
                component.Offset
            );

            EditValueInInspector<float>(
                [&]() { ImGui::DragFloat("Density", &component.Density, 0.05f); },
                [&](float density) {
                    m_commandQueue.PushBack(Command(
                        [&, newDensity = component.Density, uuid = uuidComponent.ID](void* data) {
                            auto& boxCollider = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::BoxColliderComponent>();
                            boxCollider.Density = newDensity;
                        },
                        [&, oldDensity = density, uuid = uuidComponent.ID](void* data) {
                            auto& boxCollider = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::BoxColliderComponent>();
                            boxCollider.Density = oldDensity;
                        }
                    ));
                },
                uuidComponent.ID,
                component.Density
            );
            EditValueInInspector<float>(
                [&]() { ImGui::DragFloat("Friction", &component.Friction, 0.05f); },
                [&](float friction) {
                    m_commandQueue.PushBack(Command(
                        [&, newFriction = component.Friction, uuid = uuidComponent.ID](void* data) {
                            auto& boxCollider = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::BoxColliderComponent>();
                            boxCollider.Friction = newFriction;
                        },
                        [&, oldFriction = friction, uuid = uuidComponent.ID](void* data) {
                            auto& boxCollider = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::BoxColliderComponent>();
                            boxCollider.Friction = oldFriction;
                        }
                    ));
                },
                uuidComponent.ID,
                component.Friction
            );
            EditValueInInspector<float>(
                [&]() { ImGui::DragFloat("Restitution", &component.Restitution, 0.05f); },
                [&](float restitution) {
                    m_commandQueue.PushBack(Command(
                        [&, newRestitution = component.Restitution, uuid = uuidComponent.ID](void* data) {
                            auto& boxCollider = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::BoxColliderComponent>();
                            boxCollider.Restitution = newRestitution;
                        },
                        [&, oldRestitution = restitution, uuid = uuidComponent.ID](void* data) {
                            auto& boxCollider = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::BoxColliderComponent>();
                            boxCollider.Restitution = oldRestitution;
                        }
                    ));
                },
                uuidComponent.ID,
                component.Restitution
            );
            EditValueInInspector<float>(
                [&]() { ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.05f); },
                [&](float restitutionThreshold) {
                    m_commandQueue.PushBack(Command(
                        [&, newRestitutionThreshold = component.RestitutionThreshold, uuid = uuidComponent.ID](void* data) {
                            auto& boxCollider = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::BoxColliderComponent>();
                            boxCollider.RestitutionThreshold = newRestitutionThreshold;
                        },
                        [&, oldRestitutionThreshold = restitutionThreshold, uuid = uuidComponent.ID](void* data) {
                            auto& boxCollider = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::BoxColliderComponent>();
                            boxCollider.RestitutionThreshold = oldRestitutionThreshold;
                        }
                    ));
                },
                uuidComponent.ID,
                component.RestitutionThreshold
            );
        }
    );
    DrawComponentInInspector<Nigozi::TransformComponent>("Transform", entity, m_selectionContext,
        [&](auto& component) {
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
            EditValueInInspector<float>(
                [&]() 
                {  
                    if (ImGui::DragFloat("##X", &component.Position.x, 0.1f, -INFINITY, INFINITY, "%.2f"))
                        UpdateRigidbodyTransform(entity);
                },
                [&](float positionX) {
                    m_commandQueue.PushBack(Command(
                        [&, newPositionX = component.Position.x, uuid = uuidComponent.ID](void* data) {
                            auto entity = m_currentContext->TryGetEntityByUUID(uuid);
                            auto& transform = entity.GetComponent<Nigozi::TransformComponent>();
                            transform.Position.x = newPositionX;
                            
                            UpdateRigidbodyTransform(entity);
                        },
                        [&, oldPositionX = positionX, uuid = uuidComponent.ID](void* data) {
                            auto entity = m_currentContext->TryGetEntityByUUID(uuid);
                            auto& transform = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::TransformComponent>();
                            transform.Position.x = oldPositionX;
                            
                            UpdateRigidbodyTransform(entity);
                        }
                    ));
                },
                uuidComponent.ID,
                component.Position.x
            );
            ImGui::PopItemWidth();

            // ----(1)POSITION_Y COLUMN
            ImGui::TableNextColumn();
            ImGui::Text("Y");
            ImGui::SameLine();

            ImGui::PushItemWidth(-10.0f);
            EditValueInInspector<float>(
                [&]() 
                {  
                    if (ImGui::DragFloat("##Y", &component.Position.y, 0.1f, -INFINITY, INFINITY, "%.2f")); 
                        UpdateRigidbodyTransform(entity);
                },
                [&](float positionY) {
                    m_commandQueue.PushBack(Command(
                        [&, newPositionY = component.Position.y, uuid = uuidComponent.ID](void* data) {
                            auto entity = m_currentContext->TryGetEntityByUUID(uuid);
                            auto& transform = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::TransformComponent>();
                            transform.Position.y = newPositionY;

                            UpdateRigidbodyTransform(entity);
                        },
                        [&, oldPositionY = positionY, uuid = uuidComponent.ID](void* data) {
                            auto entity = m_currentContext->TryGetEntityByUUID(uuid);
                            auto& transform = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::TransformComponent>();
                            transform.Position.y = oldPositionY;

                            UpdateRigidbodyTransform(entity);
                        }
                    ));
                },
                uuidComponent.ID,
                component.Position.y
            );
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
            EditValueInInspector<glm::vec2>(
                [&]() 
                {  
                    if (ImGui::DragFloat("##X", &component.Scale.x, 0.1f)) {
                        const bool updateScale = true;
                    }
                },
                [&](glm::vec2 scale) {
                    m_commandQueue.PushBack(Command(
                        [&, newScale = component.Scale, uuid = uuidComponent.ID](void* data) {
                            auto& transform = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::TransformComponent>();
                            transform.Scale.x = newScale.x;
                            const bool updateScale = true;
                        },
                        [&, oldScale = scale, uuid = uuidComponent.ID](void* data) {
                            auto& transform = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::TransformComponent>();
                            transform.Scale.x = oldScale.x;
                            const bool updateScale = true;
                        }
                    ));
                },
                uuidComponent.ID,
                component.Scale
            );
            ImGui::PopItemWidth();

            // ----(1)SCALE_Y COLUMN
            ImGui::TableNextColumn();
            ImGui::Text("Y");
            ImGui::SameLine();

            ImGui::PushItemWidth(-10.0f);
            EditValueInInspector<glm::vec2>(
                [&]()
                {
                    if (ImGui::DragFloat("##Y", &component.Scale.y, 0.1f)) {
                        const bool updateScale = true;
                    }
                },
                [&](glm::vec2 scale) {
                    m_commandQueue.PushBack(Command(
                        [&, newScale = component.Scale, uuid = uuidComponent.ID](void* data) {
                            auto& transform = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::TransformComponent>();
                            transform.Scale.y = newScale.y;
                            const bool updateScale = true;
                        },
                        [&, oldScale = scale, uuid = uuidComponent.ID](void* data) {
                            auto& transform = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::TransformComponent>();
                            transform.Scale.y = oldScale.y;
                            const bool updateScale = true;
                        }
                    ));
                },
                uuidComponent.ID,
                component.Scale
            );
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
            EditValueInInspector<float>(
                [&]() 
                { ImGui::DragFloat("##ROTATION", &component.Rotation, 0.1f, -360.0f, 360.0f, "%.2f", ImGuiSliderFlags_WrapAround); },
                [&](float rotation) {
                    m_commandQueue.PushBack(Command(
                        [&, newRotation = component.Rotation, uuid = uuidComponent.ID](void* data) {
                            auto& transform = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::TransformComponent>();
                            transform.Rotation = newRotation;
                            UpdateRigidbodyTransform(entity);
                        },
                        [&, oldRotation = rotation, uuid = uuidComponent.ID](void* data) {
                            auto& transform = m_currentContext->TryGetEntityByUUID(uuid).GetComponent<Nigozi::TransformComponent>();
                            transform.Rotation = oldRotation;
                            UpdateRigidbodyTransform(entity);
                        }
                    ));
                },
                uuidComponent.ID,
                component.Rotation
            );
            // ImGui::DragFloat("##ROTATION", &transform.Rotation, 0.1f, -360.0f, 360.0f, "%.2f", ImGuiSliderFlags_WrapAround);
            ImGui::PopItemWidth();

            // TRANSFORM TABLE END
            ImGui::EndTable();

            ImGui::PopStyleVar();
            ImGui::PopStyleVar();
        }
    );

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
            Nigozi::UUID nodeUuid;
            Nigozi::UUID parentUuid = Nigozi::UUID::Null;

            Nigozi::UUID sceneRootUuid = m_currentContext->GetSceneRootUUID();
            if (sceneRootUuid.GetUUID() != Nigozi::UUID::Null && m_selectionContext != entt::null) {
                parentUuid = Nigozi::Entity(m_selectionContext, m_currentContext.get()).GetUUID();
            }
            else if (sceneRootUuid.GetUUID() != Nigozi::UUID::Null && nodeUuid != sceneRootUuid) {
                parentUuid = m_currentContext->GetSceneRootUUID();
            }

            m_commandQueue.PushBack(Command(
                [&, itemSelectedIndex = itemSelectedIndex, nodeUuid = nodeUuid, parentUuid = parentUuid](void* data) {
                    Nigozi::Entity entity = m_currentContext->CreateEntity("New Node", "Empty", nodeUuid);
                    
                    entity.SetParentUUID(parentUuid);

                    AddComponent<Nigozi::SpriteRendererComponent>(entity, itemSelectedIndex == NodeTypes::SPRITE_RENDERER);
                    AddComponent<Nigozi::RigidbodyComponent>(entity, itemSelectedIndex == NodeTypes::RIGID_BODY);
                    // Rigidbody needs a box collider and we shoulnt have box collider as a seperate node
                    AddComponent<Nigozi::BoxColliderComponent>(entity, itemSelectedIndex == NodeTypes::RIGID_BODY);
                    AddComponent<Nigozi::AudioStreamPlayerComponent>(entity, itemSelectedIndex == NodeTypes::AUDIO_STREAM_PLAYER);
                    AddComponent<Nigozi::CameraComponent>(entity, itemSelectedIndex == NodeTypes::CAMERA);

                    auto& nodeType = entity.GetComponent<Nigozi::NodeTypeComponent>();

                    // TODO:
                    if (entity.GetUUID() == m_currentContext->GetSceneRootUUID())
                        nodeType.Type = Nigozi::NodeTypeComponent::Types::Scene;
                    else if (itemSelectedIndex == NodeTypes::NODE_2D)
                        nodeType.Type = Nigozi::NodeTypeComponent::Types::Node;
                    else if (itemSelectedIndex == NodeTypes::CAMERA)
                        nodeType.Type = Nigozi::NodeTypeComponent::Types::Camera;
                    else if (itemSelectedIndex == NodeTypes::SPRITE_RENDERER)
                        nodeType.Type = Nigozi::NodeTypeComponent::Types::SpriteRenderer;
                    else if (itemSelectedIndex == NodeTypes::RIGID_BODY)
                        nodeType.Type = Nigozi::NodeTypeComponent::Types::Rigidbody;
                    else if (itemSelectedIndex == NodeTypes::AUDIO_STREAM_PLAYER)
                        nodeType.Type = Nigozi::NodeTypeComponent::Types::AudioStreamPlayer;

                    if (itemSelectedIndex == NodeTypes::CAMERA) {
                        auto cameraView = m_currentContext->m_Registry.view<Nigozi::CameraComponent>();
                        for (auto [entityHandle, otherCamera] : cameraView.each()) {
                            if (entityHandle != entity.GetHandle()) {
                                otherCamera.Current = false;
                            }
                        }
                    }
                    m_selectionContext = entity.GetHandle();
                },
                [&, itemSelectedIndex = itemSelectedIndex, nodeUuid = nodeUuid](void* data) {
                    Nigozi::Entity entity = m_currentContext->TryGetEntityByUUID(nodeUuid);

                    m_selectionContext = entity.GetParent().GetHandle();
                    if (m_movingSelectionContext == entity.GetHandle()) {
                        m_movingSelectionContext = m_selectionContext;
                    }

                    if (entity != Nigozi::Entity()) {
                        entity.Destroy();
                    }
                }
            ));

            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void EditorLayer::ShowViewportPanel()
{
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;
    ImGui::Begin("##Viewport_Panel", nullptr, flags);
    bool buttonHovered = false;
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
    if (ImGui::Button("Play") && m_currentContext->GetSceneRootUUID().GetUUID() != Nigozi::UUID::Null) {
        if (SaveAllScenes()) {
            m_lastContext = m_currentContext;

            Nigozi::ScriptEngine::ReloadAssemblies();

            m_currentContext = std::make_shared<Nigozi::SceneTree>();
            m_currentContext->DeserializeScene(m_lastContext->GetFilePath());
            Nigozi::ScriptEngine::SetCurrentSceneTree(m_currentContext);

            m_currentContext->OnAttach();

            m_selectionContext = entt::null;
            m_movingSelectionContext = entt::null;


            m_editorState = EditorState::PLAY;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause") && m_editorState != EditorState::EDIT) {
        if (m_editorState == EditorState::PLAY)
            m_editorState = EditorState::PAUSE;
        else
            m_editorState = EditorState::PLAY;
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) {
        m_currentContext->OnDetach();

        m_currentContext.reset();
        m_currentContext = m_lastContext;

        m_selectionContext = entt::null;
        m_movingSelectionContext = entt::null;

        m_editorState = EditorState::EDIT;
    }
    if (ImGui::IsItemHovered()) buttonHovered = true;
    ImGui::End();
}

void EditorLayer::ShowViewport()
{
    ImGui::Begin("Viewport");
    ImGuiTabBarFlags tab_bar_flags = 
        ImGuiTabBarFlags_AutoSelectNewTabs | 
        ImGuiTabBarFlags_Reorderable | 
        ImGuiTabBarFlags_FittingPolicyResizeDown;
    std::shared_ptr<Nigozi::SceneTree> closeContext;
    
    if (m_editorState == EditorState::EDIT && ImGui::BeginTabBar("SceneTab", tab_bar_flags)) {
        for (const auto& context : m_sceneTreeContexts) {
            std::string name = "*";
            if (context->HasFilePath()) {
                name = context->GetFilePath().filename().string();
            }
            bool contextOpen = true;
            ImGui::PushID((std::to_string((uint64_t)context.get()).c_str()));
            if (ImGui::BeginTabItem(name.c_str(), &contextOpen)) {
                if (m_currentContext != context) {
                    m_currentContext = context;
                }
                ImGui::EndTabItem();
            }
            if (!contextOpen) {
                closeContext = context;
            }
            ImGui::PopID();
        }
        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
            m_currentContext = std::make_shared<Nigozi::SceneTree>();
            m_sceneTreeContexts.push_back(m_currentContext);
        }
        ImGui::EndTabBar();
    }
    if (m_editorState == EditorState::EDIT && closeContext.get()) {
        CloseSceneTab(closeContext);
    }

    m_viewportSize = ImGui::GetContentRegionAvail();

    ImVec2 viewportFrameSize = ImGui::GetContentRegionMax();
    
    ImVec2 padding{ viewportFrameSize.x - m_viewportSize.x, viewportFrameSize.y - m_viewportSize.y };
    padding.y += ImGui::GetFrameHeight();
    
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

    ImVec2 availableFrameSize = ImGui::GetContentRegionAvail();
    ImVec2 offset
    { 
        (availableFrameSize.x - m_viewportSize.x) / 2.0f,
        (availableFrameSize.y - m_viewportSize.y) / 2.0f 
    };

    m_viewportPosition = glm::vec2(ImGui::GetWindowPos().x + padding.x + offset.x, ImGui::GetWindowPos().y + padding.y + offset.y);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset.x);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset.y);
    
    ImGui::Image((uint64_t)(p_viewportBuffer->GetColorAttachment()), m_viewportSize, ImVec2(0, 1), ImVec2(1, 0));
    
    // VIEWPORT_FILE_TREE_ITEM_DRAG_DROP
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && ImGui::GetMouseDragDelta(ImGuiMouseButton_Left).x != 0.0f) {

        ImVec2 min = ImGui::GetWindowPos();
        ImVec2 max = { min.x + ImGui::GetWindowWidth(), min.y + ImGui::GetWindowHeight() };
        
        ImVec4* col = ImGui::GetStyle().Colors;
        ImVec4 frameBg = col[ImGuiCol_TabSelectedOverline];
        frameBg.x *= 255.0f;
        frameBg.y *= 255.0f;
        frameBg.z *= 255.0f;
        frameBg.w *= 255.0f;

        ImGui::GetForegroundDrawList()->AddRect(min, max, IM_COL32(frameBg.x, frameBg.y, frameBg.z, frameBg.w), 0.0f, 0, 2.0f);
    }
    if (ImGui::IsWindowHovered() && ImGui::BeginDragDropTarget()) {

        std::filesystem::path filePath((char*)ImGui::GetDragDropPayload()->Data);
        if (!filePath.empty() && filePath.extension() == ".ngscn") {
            LoadScene(filePath);
        }
        ImGui::EndDragDropTarget();
    }

    m_viewportHovered = ImGui::IsWindowHovered();

    ImGui::End();
}

void EditorLayer::ShowCreateOrOpenProjectModal()
{
    if (Nigozi::Project::s_ProjectDir.empty() && !ImGui::IsPopupOpen("Load project...")) {
        ImGui::OpenPopup("Load project...");
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Load project...", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        
        ImGui::Text("No projects found to load");

        if (Nigozi::Project::s_ProjectDir.empty()) {
            if (ImGui::Button("Create Project")) {
                std::filesystem::path projectDir = Nigozi::FileDialogue::OpenFolderDialog();
                if (!projectDir.empty() && std::filesystem::is_directory(projectDir)) {
                    Nigozi::Project::s_ProjectDir = projectDir;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Load Project")) {
                std::filesystem::path projectDir = Nigozi::FileDialogue::OpenFolderDialog();
                if (!projectDir.empty() && std::filesystem::is_directory(projectDir)) {
                    Nigozi::Project::s_ProjectDir = projectDir;

                    ProjectUtils::DeserializeProjectMetadata();
                    std::filesystem::path slnPath = Nigozi::Project::s_ProjectDir / (Nigozi::Project::s_ProjectName + ".sln");
                    if (!std::filesystem::exists(slnPath)) {
                        ProjectUtils::RecreateScriptsProject();
                    }

                    Nigozi::ScriptEngine::Initialize("src/res/scripts");

                    ImGui::CloseCurrentPopup();
                }
            }
        }
        else {
            std::filesystem::path finalPath = Nigozi::Project::s_ProjectDir / Nigozi::Project::s_ProjectName;
            ImGui::Text("Project dir: ");
            ImGui::Text(finalPath.string().c_str());

            if (Nigozi::Project::s_ProjectName.capacity() < 96)
                Nigozi::Project::s_ProjectName.reserve(96);
            if (ImGui::InputText("Project Name", Nigozi::Project::s_ProjectName.data(), Nigozi::Project::s_ProjectName.capacity()))
            {
                Nigozi::Project::s_ProjectName.resize(strlen(Nigozi::Project::s_ProjectName.data()));
            }
            if (ImGui::Button("Create") && !Nigozi::Project::s_ProjectName.empty()) {
                ProjectUtils::Error error = ProjectUtils::CreateProject(Nigozi::Project::s_ProjectDir, Nigozi::Project::s_ProjectName);
                if (error == ProjectUtils::Error::InvalidProjectDir) {
                    NG_CLIENT_LOG_ERROR("Invalid project directory!");
                }
                else if (error == ProjectUtils::Error::ProjectPathAlreadyExists) {
                    NG_CLIENT_LOG_ERROR("Project path already exists!");
                }
                else if (error == ProjectUtils::Error::SubprocessFailed) {
                    NG_CLIENT_LOG_ERROR("Premake subprocess failed!");
                }
                else {
                    Nigozi::ScriptEngine::Initialize("src/res/scripts");
                    ImGui::CloseCurrentPopup();
                }
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Exit")) {
            Nigozi::Application::Close();
        }
        ImGui::EndPopup();
    }
}

void EditorLayer::CloseSceneTab(const std::shared_ptr<Nigozi::SceneTree>& sceneContext)
{
    auto it = std::find(m_sceneTreeContexts.begin(), m_sceneTreeContexts.end(), sceneContext);
    if (it == m_sceneTreeContexts.end()) {
        NG_CLIENT_LOG_ERROR("Couldn't find context in vector! {}", (void*)sceneContext.get());
        return;
    }

    if (m_sceneTreeContexts.size() == 1 && !sceneContext->HasFilePath()) {
        return;
    }

    m_commandQueue.PushBack(Command(
        [&, filePath = sceneContext->GetFilePath()](void* data) {
            for (auto it = m_sceneTreeContexts.begin(); it != m_sceneTreeContexts.end(); ++it) {
                if ((*it)->GetFilePath() == filePath) {
                    if (it == m_sceneTreeContexts.begin() && m_sceneTreeContexts.size() == 1) {
                        m_currentContext = std::make_shared<Nigozi::SceneTree>();
                        m_sceneTreeContexts.push_back(m_currentContext);
                    }
                    else if (it == m_sceneTreeContexts.begin()) {
                        m_currentContext = (*std::next(it));
                    }
                    else {
                        m_currentContext = (*std::prev(it));
                    }
                    m_sceneTreeContexts.erase(it);
                    break;
                }
            }
        },
        [&, filePath = sceneContext->GetFilePath()](void* data) {
            m_currentContext = std::make_shared<Nigozi::SceneTree>();
            m_currentContext->DeserializeScene(filePath);
            m_sceneTreeContexts.push_back(m_currentContext);
        })
    );
}

bool EditorLayer::SaveAllScenes()
{
    for (const auto& context : m_sceneTreeContexts) {
        if (!SaveScene(context)) {
            return false;
        }
    }
    return true;
}

bool EditorLayer::SaveScene(const std::shared_ptr<Nigozi::SceneTree>& context)
{
    if (context->HasFilePath()) {
        return context->SerializeScene();
    }
    std::filesystem::path savePath = Nigozi::FileDialogue::OpenSaveDialog("ngscn");
    if (!savePath.empty()) {
        return context->SerializeScene(savePath);
    }
    return false;
}

bool EditorLayer::SaveCurrentScene()
{
    if (m_currentContext->HasFilePath()) {
        return m_currentContext->SerializeScene();
    }
    std::filesystem::path savePath = Nigozi::FileDialogue::OpenSaveDialog("ngscn");
    if (!savePath.empty()) {
        return m_currentContext->SerializeScene(savePath);
    }
    return false;
}

bool EditorLayer::SaveCurrentSceneAs()
{
    std::filesystem::path savePath = Nigozi::FileDialogue::OpenSaveDialog("ngscn");
    if (!savePath.empty()) {
        return m_currentContext->SerializeScene(savePath);
    }
    return false;
}

void EditorLayer::LoadScene()
{
    std::filesystem::path scenePath = Nigozi::FileDialogue::OpenFileDialog("ngscn");
    if (scenePath.empty()) {
        return;
    }
    for (const auto& context : m_sceneTreeContexts) {
        if (context->GetFilePath() == scenePath) {
            return;
        }
    }

    m_commandQueue.PushBack(Command(
        [&, filePath = scenePath](void* data) {
            auto sceneContext = std::make_shared<Nigozi::SceneTree>();
            m_currentContext = sceneContext;
            m_sceneTreeContexts.push_back(m_currentContext);
            m_currentContext->DeserializeScene(filePath);
        },
        [&, filePath = scenePath](void* data) {
            for (auto it = m_sceneTreeContexts.begin(); it < m_sceneTreeContexts.end(); ++it) {
                if ((*it)->GetFilePath() == filePath) {
                    m_sceneTreeContexts.erase(it);
                    m_selectionContext = entt::null;
                    m_movingSelectionContext = entt::null;
                    break;
                }
            }
        })
    );
}

void EditorLayer::LoadScene(std::filesystem::path& scenePath)
{
    if (scenePath.empty()) {
        return;
    }
    for (const auto& context : m_sceneTreeContexts) {
        if (context->GetFilePath() == scenePath) {
            return;
        }
    }

    m_commandQueue.PushBack(Command(
        [&, filePath = scenePath](void* data) {
            auto sceneContext = std::make_shared<Nigozi::SceneTree>();
            m_currentContext = sceneContext;
            m_sceneTreeContexts.push_back(m_currentContext);
            m_currentContext->DeserializeScene(filePath);
        },
        [&, filePath = scenePath](void* data) {
            for (auto it = m_sceneTreeContexts.begin(); it < m_sceneTreeContexts.end(); ++it) {
                if ((*it)->GetFilePath() == filePath) {
                    m_sceneTreeContexts.erase(it);
                    m_selectionContext = entt::null;
                    m_movingSelectionContext = entt::null;
                    break;
                }
            }
        })
    );
}
