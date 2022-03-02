#include "Game.hpp"

#include "State.hpp"

Game::Game() {
    // TODO:: Create Shader
    basic_shader = std::make_unique<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    alpha_shader = std::make_unique<Shader>("assets/shaders/alpha.vert", "assets/shaders/alpha.frag");
    lighting_shader = std::make_unique<Shader>("assets/shaders/lighting.vert", "assets/shaders/lighting.frag");
    screen_shader = std::make_unique<Shader>("assets/shaders/screen.vert", "assets/shaders/screen.frag");

    // TODO:: Model Matrix Stack
    model = std::make_unique<MatrixStack>();

    // TODO:: Build a world (Entity, Camera, Geometry)
    state.world = std::make_unique<World>();
    state.world->create();

    // Create Renderer
    master_renderer = std::make_unique<MasterRenderer>();
    entities_renderer = std::make_unique<EntitiesRenderer>();
}

void Game::RendererInit() {
    master_renderer->Initialize();
    entities_renderer->Initialize();
}

void Game::Update(float dt) {
    state.world->my_camera->Update(dt);
    state.world->ortho_x_camera->UpdateTargetPosition(state.world->my_camera->position);
    state.world->ortho_y_camera->UpdateTargetPosition(state.world->my_camera->position);
    state.world->ortho_z_camera->UpdateTargetPosition(state.world->my_camera->position);

    state.world->UpdateViewVolumeVertices();

    state.world->earth->rotate.y += state.world->speed * dt;

    state.world->my_spotlight->position = state.world->my_camera->position;
    state.world->my_spotlight->direction = state.world->my_camera->front;

    state.world->rick_roll->UpdatePosition(glm::vec3(0.2f, 1.0f, 0.0f), dt);
    state.world->rick_roll->UpdateRotation(glm::vec3(0.0f, 15.0f, 10.0f), dt);
}

void Game::Render(const std::unique_ptr<Camera>& current_camera, float dt) {

    // 是否開啟 Back Face Culling
    if (state.world->culling) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }

    if (state.world->wire_mode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    current_camera->SetViewPort();
    glm::mat4 view = current_camera->View();
    glm::mat4 projection = current_camera->Projection();

    // Draw Light Ball
    basic_shader->Use();
    basic_shader->SetMat4("view", view);
    basic_shader->SetMat4("projection", projection);
    if (state.world->my_directional_light->enable) {
        model->Push();
        model->Save(glm::translate(model->Top(), state.world->my_directional_light->direction * -1.0f));
        basic_shader->SetVec3("objectColor", state.world->my_directional_light->color);
        basic_shader->SetMat4("model", model->Top());
        state.world->my_sphere->Draw();
        model->Pop();
    }
    for (int i = 0; i < state.world->my_point_lights.size(); ++i) {
        if (state.world->my_point_lights[i]->enable) {
            model->Push();
            model->Save(glm::translate(model->Top(), state.world->my_point_lights[i]->position));
            basic_shader->SetVec3("objectColor", state.world->my_point_lights[i]->color);
            basic_shader->SetMat4("model", model->Top());
            state.world->my_sphere->Draw();
            model->Pop();
        }
    }

    // Draw Entities
    lighting_shader->Use();
    lighting_shader->SetMat4("view", view);
    lighting_shader->SetMat4("projection", projection);
    lighting_shader->SetBool("useLighting", true);
    lighting_shader->SetBool("useBlinnPhong", true);
    lighting_shader->SetBool("useTexture", false);
    lighting_shader->SetInt("diffuse_texture", 0);

    // Setting Lighting
    lighting_shader->SetFloat("shininess", state.world->shininess);
    lighting_shader->SetVec3("viewPos", current_camera->position);

    lighting_shader->SetVec3("lights[0].direction", state.world->my_directional_light->direction);
    lighting_shader->SetVec3("lights[0].ambient", state.world->my_directional_light->ambient);
    lighting_shader->SetVec3("lights[0].diffuse", state.world->my_directional_light->diffuse);
    lighting_shader->SetVec3("lights[0].specular", state.world->my_directional_light->specular);
    lighting_shader->SetBool("lights[0].enable", state.world->my_directional_light->enable);
    lighting_shader->SetInt( "lights[0].caster", state.world->my_directional_light->caster);

    lighting_shader->SetVec3("lights[1].position", state.world->my_spotlight->position);
    lighting_shader->SetVec3("lights[1].direction", state.world->my_spotlight->direction);
    lighting_shader->SetVec3("lights[1].ambient", state.world->my_spotlight->ambient);
    lighting_shader->SetVec3("lights[1].diffuse", state.world->my_spotlight->diffuse);
    lighting_shader->SetVec3("lights[1].specular", state.world->my_spotlight->specular);
    lighting_shader->SetFloat("lights[1].constant", state.world->my_spotlight->constant);
    lighting_shader->SetFloat("lights[1].linear", state.world->my_spotlight->linear);
    lighting_shader->SetFloat("lights[1].quadratic", state.world->my_spotlight->quadratic);
    lighting_shader->SetFloat("lights[1].cutoff", glm::cos(glm::radians(state.world->my_spotlight->cutoff)));
    lighting_shader->SetFloat("lights[1].outerCutoff", glm::cos(glm::radians(state.world->my_spotlight->outer_cutoff)));
    lighting_shader->SetBool("lights[1].enable", state.world->my_spotlight->enable);
    lighting_shader->SetInt("lights[1].caster", state.world->my_spotlight->caster);
    
    for (int i = 0; i < state.world->my_point_lights.size(); ++i) {
        lighting_shader->SetVec3("lights[" + std::to_string(i + 2) + "].position", state.world->my_point_lights[i]->position);
        lighting_shader->SetVec3("lights[" + std::to_string(i + 2) + "].ambient", state.world->my_point_lights[i]->ambient);
        lighting_shader->SetVec3("lights[" + std::to_string(i + 2) + "].diffuse", state.world->my_point_lights[i]->diffuse);
        lighting_shader->SetVec3("lights[" + std::to_string(i + 2) + "].specular", state.world->my_point_lights[i]->specular);
        lighting_shader->SetFloat("lights[" + std::to_string(i + 2) + "].constant", state.world->my_point_lights[i]->constant);
        lighting_shader->SetFloat("lights[" + std::to_string(i + 2) + "].linear", state.world->my_point_lights[i]->linear);
        lighting_shader->SetFloat("lights[" + std::to_string(i + 2) + "].quadratic", state.world->my_point_lights[i]->quadratic);
        lighting_shader->SetBool("lights[" + std::to_string(i + 2) + "].enable", state.world->my_point_lights[i]->enable);
        lighting_shader->SetInt("lights[" + std::to_string(i + 2) + "].caster", state.world->my_point_lights[i]->caster);
    }



    model->Push();
    model->Save(glm::translate(model->Top(), state.world->my_camera->position));
    model->Save(glm::rotate(model->Top(), glm::radians(-state.world->my_camera->yaw), glm::vec3(0.0, 1.0, 0.0)));
    model->Save(glm::rotate(model->Top(), glm::radians(state.world->my_camera->pitch), glm::vec3(1.0, 0.0, 0.0)));
    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, 0.0f, 1.0f)));
    // 繪製攝影機三軸
    DrawAxes(20.0f);
    model->Pop();
    
    model->Push();
    model->Save(glm::translate(model->Top(), state.world->my_camera->position));
    model->Save(glm::rotate(model->Top(), glm::radians(-state.world->my_camera->yaw), glm::vec3(0.0, 1.0, 0.0)));
    model->Save(glm::rotate(model->Top(), glm::radians(state.world->my_camera->pitch), glm::vec3(1.0, 0.0, 0.0)));
    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, 0.0f, 5.5f)));
    model->Save(glm::scale(model->Top(), glm::vec3(8.0f, 5.0f, 10.0f)));
    lighting_shader->SetVec3("objectColor", glm::vec3(0.2f));
    lighting_shader->SetMat4("model", model->Top());
    // 繪製攝影機本體
    state.world->my_cube->Draw();
    model->Pop();

    // TODO:: Model Matrix
    lighting_shader->SetBool("useTexture", true);
    entities_renderer->Render(state.world->rick_roll.get(), lighting_shader.get());
    lighting_shader->SetBool("useTexture", false);
    //    model->Push();
    //    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, 25.0f, 0.0f)));
    //    model->Save(glm::scale(model->Top(), glm::vec3(10.0f)));
    //    lighting_shader->SetMat4("model", model->Top());
    //    state.world->my_cube->Draw();
    //    model->Pop();

    model->Push();
    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, -50.0f, 0.0f)));
    model->Save(glm::scale(model->Top(), glm::vec3(100.0f)));
    lighting_shader->SetVec3("objectColor", glm::vec3(42 / 255.0f, 219 / 255.0f, 89 / 255.0f));
    lighting_shader->SetMat4("model", model->Top());
    state.world->my_cube->Draw();
    model->Pop();

    model->Push();
    model->Push();
    model->Save(glm::translate(model->Top(), state.world->sun->position));
    model->Push();
    model->Save(glm::scale(model->Top(), state.world->sun->scale));
    lighting_shader->SetVec3("objectColor", glm::vec3(1.0f, 0.811764706f, 0.301960784f));
    lighting_shader->SetMat4("model", model->Top());
    state.world->my_sphere->Draw();
    model->Pop();
    DrawAxes();
    model->Pop();

    model->Push();
    model->Save(glm::rotate(model->Top(), glm::radians(state.world->earth->rotate.y / 365.0f), glm::vec3(0.0, 1.0, 0.0)));
    model->Save(glm::translate(model->Top(), state.world->earth->position));
    model->Save(glm::rotate(model->Top(), glm::radians(23.5f), glm::vec3(1.0, 0.0, 0.0)));
    model->Save(glm::rotate(model->Top(), glm::radians(state.world->earth->rotate.y), glm::vec3(0.0, 1.0, 0.0)));
    DrawAxes();

    model->Push();
    model->Save(glm::scale(model->Top(), state.world->earth->scale));
    lighting_shader->SetVec3("objectColor", glm::vec3(0.137254902f, 0.274509804f, 0.968627451f));
    lighting_shader->SetMat4("model", model->Top());
    state.world->my_sphere->Draw();
    model->Pop();

    model->Push();
    model->Save(glm::rotate(model->Top(), glm::radians(state.world->earth->rotate.y / 48.0f), glm::vec3(0.0, 1.0, 0.0)));
    model->Save(glm::translate(model->Top(), state.world->moon->position));
    DrawAxes();

    model->Save(glm::scale(model->Top(), state.world->moon->scale));
    lighting_shader->SetVec3("objectColor", glm::vec3(0.447058824f, 0.450980392f, 0.478431373f));
    lighting_shader->SetMat4("model", model->Top());
    state.world->my_sphere->Draw();

    model->Pop();
    model->Pop();
    model->Pop();



    alpha_shader->Use();
    alpha_shader->SetMat4("view", view);
    alpha_shader->SetMat4("projection", projection);
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
    state.world->my_camera->ProcessKeyboard();
    state.world->my_camera->ProcessMouseMovement();
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
                state.world->my_camera->ToggleMouseControl();
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

    model->Push();
    model->Save(glm::translate(model->Top(), glm::vec3(length / 2.0f, 0.0f, 0.0f)));
    model->Save(glm::scale(model->Top(), glm::vec3(length, 0.5f, 0.5f)));
    lighting_shader->SetVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
    lighting_shader->SetMat4("model", model->Top());
    state.world->my_cube->Draw();
    model->Pop();

    model->Push();
    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, length / 2.0f, 0.0f)));
    model->Save(glm::scale(model->Top(), glm::vec3(0.5f, length, 0.5f)));
    lighting_shader->SetVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f));
    lighting_shader->SetMat4("model", model->Top());
    state.world->my_cube->Draw();
    model->Pop();

    model->Push();
    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, 0.0f, length / 2.0f)));
    model->Save(glm::scale(model->Top(), glm::vec3(0.5f, 0.5f, length)));
    lighting_shader->SetVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));
    lighting_shader->SetMat4("model", model->Top());
    state.world->my_cube->Draw();
    model->Pop();
}