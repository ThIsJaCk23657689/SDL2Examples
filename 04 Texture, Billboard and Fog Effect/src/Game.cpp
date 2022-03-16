#include "Game.hpp"

#include "State.hpp"

Game::Game() {
    // Create Shader
    basic_shader = std::make_unique<BasicShader>();
    // lighting_shader = std::make_unique<LightningShader>();
    alpha_shader = std::make_unique<AlphaShader>();
    // screen_shader = std::make_unique<Shader>("assets/shaders/screen.vert", "assets/shaders/screen.frag");

    // Create Renderer
    master_renderer = std::make_unique<MasterRenderer>();
    // entities_renderer = std::make_unique<EntitiesRenderer>(lighting_shader.get());

    // Model Matrix Stack
    model = std::make_unique<MatrixStack>();

    state.world = std::make_unique<World>();
    state.world->Create();
}

void Game::RendererInit() {
    // 在每一次的 Game loop 都會執行，且在分割畫面之前
    master_renderer->Initialize();
}

void Game::Update(float dt) {
    // Update the Player
    state.world->me->Update(dt);

    // Update cameras
    state.world->my_camera->Update(dt);
    state.world->ortho_x_camera->UpdateTargetPosition(state.world->my_camera->position);
    state.world->ortho_y_camera->UpdateTargetPosition(state.world->my_camera->position);
    state.world->ortho_z_camera->UpdateTargetPosition(state.world->my_camera->position);

    state.world->UpdateViewVolumeVertices();

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


    // TODO: 繪製 xyz 三軸
//    model->Push();
//    model->Save(glm::translate(model->Top(), state.world->my_camera->position));
//    model->Save(glm::rotate(model->Top(), glm::radians(-state.world->my_camera->yaw), glm::vec3(0.0, 1.0, 0.0)));
//    model->Save(glm::rotate(model->Top(), glm::radians(state.world->my_camera->pitch), glm::vec3(1.0, 0.0, 0.0)));
//    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, 0.0f, 1.0f)));
//    // 繪製攝影機三軸
//    model->Pop();
//
//    model->Push();
//    model->Save(glm::translate(model->Top(), state.world->my_camera->position));
//    model->Save(glm::rotate(model->Top(), glm::radians(-state.world->my_camera->yaw), glm::vec3(0.0, 1.0, 0.0)));
//    model->Save(glm::rotate(model->Top(), glm::radians(state.world->my_camera->pitch), glm::vec3(1.0, 0.0, 0.0)));
//    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, 0.0f, 5.5f)));
//    model->Save(glm::scale(model->Top(), glm::vec3(8.0f, 5.0f, 10.0f)));
//    lighting_shader->SetVec3("objectColor", glm::vec3(0.2f));
//    lighting_shader->SetMat4("model", model->Top());
//    // 繪製攝影機本體
//    state.world->my_cube->Draw();
//    model->Pop();
//
//
//    // Sun
//    model->Push();
    //    model->Push();
    //    model->Save(glm::translate(model->Top(), state.world->sun->position));
        //    model->Push();
        //    model->Save(glm::scale(model->Top(), state.world->sun->scale));
        //    lighting_shader->SetVec3("objectColor", glm::vec3(1.0f, 0.811764706f, 0.301960784f));
        //    lighting_shader->SetMat4("model", model->Top());
        //    state.world->my_sphere->Draw();
        //    model->Pop();
    //    model->Pop();
    //
    //    // Earth
    //    model->Push();
        //    model->Save(glm::rotate(model->Top(), glm::radians(state.world->earth->rotate.y / 365.0f), glm::vec3(0.0, 1.0, 0.0)));
        //    model->Save(glm::translate(model->Top(), state.world->earth->position));
        //    model->Save(glm::rotate(model->Top(), glm::radians(23.5f), glm::vec3(1.0, 0.0, 0.0)));
        //    model->Save(glm::rotate(model->Top(), glm::radians(state.world->earth->rotate.y), glm::vec3(0.0, 1.0, 0.0)));
    //
        //    model->Push();
        //    model->Save(glm::scale(model->Top(), state.world->earth->scale));
        //    lighting_shader->SetVec3("objectColor", glm::vec3(0.137254902f, 0.274509804f, 0.968627451f));
        //    lighting_shader->SetMat4("model", model->Top());
        //    state.world->my_sphere->Draw();
        //    model->Pop();
    //
    //        model->Push();
        //    model->Save(glm::rotate(model->Top(), glm::radians(state.world->earth->rotate.y / 48.0f), glm::vec3(0.0, 1.0, 0.0)));
        //    model->Save(glm::translate(model->Top(), state.world->moon->position));
        //    DrawAxes();
    //
        //    model->Save(glm::scale(model->Top(), state.world->moon->scale));
        //    lighting_shader->SetVec3("objectColor", glm::vec3(0.447058824f, 0.450980392f, 0.478431373f));
        //    lighting_shader->SetMat4("model", model->Top());
        //    state.world->my_sphere->Draw();
    //
    //      model->Pop();
    //    model->Pop();
//    model->Pop();

    // TODO: 把 View Volume 納入 Alpha Renderer
    alpha_shader->Start();
    alpha_shader->SetViewAndProj(current_camera);
    // 繪製 View Volume
    model->Push();
    alpha_shader->SetVec4("objectColor", glm::vec4(0.1f, 0.1f, 0.1f, 0.9f));
    alpha_shader->SetMat4("model", model->Top());
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    state.world->view_volume->Draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    alpha_shader->SetVec4("objectColor", glm::vec4(0.889f, 0.889f, 0.889f, 0.3f));
    state.world->view_volume->Draw();
    model->Pop();
}

void Game::Destroy() {
    basic_shader->Destroy();
    alpha_shader->Destroy();
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

void Game::DrawAxes(float length) {
    if (!state.world->draw_axes) {
        return;
    }

//    model->Push();
//    model->Save(glm::translate(model->Top(), glm::vec3(length / 2.0f, 0.0f, 0.0f)));
//    model->Save(glm::scale(model->Top(), glm::vec3(length, 0.5f, 0.5f)));
//    lighting_shader->SetVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
//    lighting_shader->SetMat4("model", model->Top());
//    state.world->my_cube->Draw();
//    model->Pop();
//
//    model->Push();
//    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, length / 2.0f, 0.0f)));
//    model->Save(glm::scale(model->Top(), glm::vec3(0.5f, length, 0.5f)));
//    lighting_shader->SetVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f));
//    lighting_shader->SetMat4("model", model->Top());
//    state.world->my_cube->Draw();
//    model->Pop();
//
//    model->Push();
//    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, 0.0f, length / 2.0f)));
//    model->Save(glm::scale(model->Top(), glm::vec3(0.5f, 0.5f, length)));
//    lighting_shader->SetVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));
//    lighting_shader->SetMat4("model", model->Top());
//    state.world->my_cube->Draw();
//    model->Pop();
}