#include "Game.hpp"

#include "Texture/TextureManager.hpp"
#include "State.hpp"

Game::Game() {
    // Create Shader
    // screen_shader = std::make_unique<Shader>("assets/shaders/screen.vert", "assets/shaders/screen.frag");

    // Create Renderer
    master_renderer = std::make_unique<MasterRenderer>();

    state.world = std::make_unique<World>();
    state.world->Create();

    // Create Framebuffer and Renderbuffer
    main_renderbuffer = std::make_unique<RenderBuffer>(state.window->width, state.window->height);
    main_framebuffer = std::make_unique<FrameBuffer>();
    main_framebuffer->BindTexture2D(TextureManager::GetTexture2D("PostProcessing"));
    main_framebuffer->BindRenderBuffer(main_renderbuffer);
    main_framebuffer->CheckComplete();
}

void Game::RendererInit() {
    // 在每一次的 Game loop 都會執行，且在分割畫面之前
    main_framebuffer->Bind();
    master_renderer->Initialize();
}

void Game::RenderScreen() {
    main_framebuffer->UnBind();
    master_renderer->RenderScreen();
}

void Game::Update(float dt) {
    // Update the Player
    state.world->me->Update(dt);

    // Update cameras
    state.world->my_camera->Update(dt);
    state.world->ortho_x_camera->UpdateTargetPosition(state.world->my_camera->position);
    state.world->ortho_y_camera->UpdateTargetPosition(state.world->my_camera->position);
    state.world->ortho_z_camera->UpdateTargetPosition(state.world->my_camera->position);

    state.world->billboard->UpdateBillboard(state.world->my_camera);
    state.world->view_volume->UpdateViewVolume(state.world->my_camera);

    // Update the spotlight
    state.world->my_spotlight->Update(dt);

    state.world->earth.UpdateRotation(glm::vec3(0.0f, 80.0f, 0.0f), dt);
    state.world->moon.UpdateRotation(glm::vec3(0.0f, 30.0f, 0.0f), dt);

    for (auto& rick : state.world->rick_rolls) {
        rick.UpdateRotation(glm::vec3(0.0f, 80.0f, 10.0f), dt);
    }
}

void Game::Render(const std::unique_ptr<Camera>& current_camera, float dt) {
    // 基本上就是每一偵都會執行此函數，如果說畫面有切割的話，那同一次 Game loop 之中會 repeat 多次。
    master_renderer->Render(current_camera);

}

void Game::Destroy() {
    state.world->Destroy();
}

void Game::HandleEvents() {
    // 將所有事件拉出來到 vector 中
    PollEvents();

    // 透過迴圈一個一個去跑 vector 來跑事件處理
    for (auto it = std::cbegin(events); it != std::cend(events); it++) {
        // 先處理 ImGui 的事件
        state.ui->ProcessEvent(*it);

        // 再來處理 SDL 自己的事件
        ProcessEvents(*it, state.ui->WantCaptureEvent);
    }

    // 如果 ImGui 佔用了滑鼠與鍵盤時，就不會執行下面的事件控制
    if (state.ui->WantCaptureEvent) {
        return;
    }

    // 執行攝影機的控制，這邊事件觸發並不是透過 SDL_Event ，而是透過像是 SDL_GetKeyboardState() 和 SDL_GetRelativeMouseState() 去控制的。
    // Camera Event Handler
    state.world->me->HandleEvents();

    // state.world->my_camera->ProcessKeyboard();
    // state.world->my_camera->ProcessMouseMovement();
}

void Game::PollEvents() {
    events.clear();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        events.push_back(event);
    }
}

void Game::ProcessEvents(const SDL_Event &event, bool ByPassSceneEvents) {
    // 優先度大於 ImGui 的事件：離開程式
    GlobalEvents(event);

    if (ByPassSceneEvents) {
        return;
    }

    // 優先度小於 ImGui
    SceneEvents(event);
}

void Game::GlobalEvents(const SDL_Event &event) {
    switch (event.type) {
        case SDL_QUIT:
            state.window->should_close = true;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_q) {
                if (event.key.keysym.mod & KMOD_CTRL) {
                    state.window->should_close = true;
                }
            }
            if (event.key.keysym.sym == SDLK_TAB) {
                state.world->me->ToggleMouseControl();
            }
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                // 更新視窗長寬
                SDL_GetWindowSize(state.window->handler, &state.window->width, &state.window->height);

                // 更新 Framebuffer Texture 和 Renderer buffer
                UpdateFramebuffer();
            }
            break;
    }
}

void Game::SceneEvents(const SDL_Event &event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            OnKeyDownEvent(event.key);
            break;
        case SDL_MOUSEBUTTONDOWN:
            OnMouseButtonEvent(event.button);
            break;
        case SDL_MOUSEMOTION:
            OnMouseMotionEvent(event.motion);
            break;
        case SDL_MOUSEWHEEL:
            OnMouseWheelEvent(event.wheel);
            break;
        case SDL_WINDOWEVENT:
            OnWindowEvent(event.window);
            break;
    }
}

void Game::OnKeyDownEvent(const SDL_KeyboardEvent& e) {
    switch (e.keysym.sym) {
        case SDLK_f:
            state.world->my_spotlight->enable = !state.world->my_spotlight->enable;
            break;
        case SDLK_x:
            state.world->draw_axes = !state.world->draw_axes;
            break;
        case SDLK_1:
            current_screen_mode = ScreenMode::OrthoX;
            break;
        case SDLK_2:
            current_screen_mode = ScreenMode::OrthoY;
            break;
        case SDLK_3:
            current_screen_mode = ScreenMode::OrthoZ;
            break;
        case SDLK_4:
            current_screen_mode = ScreenMode::Prspct;
            break;
        case SDLK_5:
            current_screen_mode = ScreenMode::Divide;
            break;
        default:
            break;
    }
}

void Game::OnMouseButtonEvent(const SDL_MouseButtonEvent& e) {
    switch (e.button) {
        case SDL_BUTTON_LEFT:
            break;
        case SDL_BUTTON_MIDDLE:
            break;
        case SDL_BUTTON_RIGHT:
            break;
    }
}

void Game::OnMouseMotionEvent(const SDL_MouseMotionEvent& e) {
    switch (e.state) {
        case SDL_BUTTON_LMASK:
            break;
        case SDL_BUTTON_RMASK:
            break;
        default:
            break;
    }
}

void Game::OnMouseWheelEvent(const SDL_MouseWheelEvent& e) {
    state.world->my_camera->ProcessMouseScroll(e.y);
}

void Game::OnWindowEvent(const SDL_WindowEvent& e) {
//    switch (e.event) {
//        default:
//            break;
//    }
}

void Game::UpdateFramebuffer() {
    // 當視窗大小改變時，每次都必須要重新更改 Texture 與 Renderbuffer 的大小
    main_renderbuffer->Resize(state.window->width, state.window->height);

    Texture2D screen = TextureManager::GetTexture2D("PostProcessing");
    screen.Bind();
    screen.Generate(GL_RGB, GL_RGB, state.window->width, state.window->height, nullptr, false);
    screen.UnBind();
}