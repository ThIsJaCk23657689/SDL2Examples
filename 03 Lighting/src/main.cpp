#include <SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>

#include "MatrixStack.hpp"
#include "Shader.hpp"
#include "UI.hpp"
#include "World.hpp"

enum ScreenMode : unsigned int {
    Divide = 0,
    OrthoX = 1,
    OrthoY = 2,
    OrthoZ = 3,
    Prspct = 4,
};

bool is_quit = false;
float current_time = 0.0f;
float delta_time = 0.0f;
float last_time = 0.0f;
std::vector<SDL_Event> events = {};

// 視窗屬性
int window_width = 800;
int window_height = 600;
SDL_Window* window = nullptr;
ScreenMode current_screen_mode = ScreenMode::Divide;

std::unique_ptr<UI> my_ui = nullptr;
//std::unique_ptr<Shader> my_shader = nullptr;
std::unique_ptr<Shader> alpha_shader = nullptr;
std::unique_ptr<Shader> lighting_shader = nullptr;
std::unique_ptr<MatrixStack> model = nullptr;

// 事件處理相關
void OnWindowEvent(const SDL_WindowEvent& e);
void OnKeyDownEvent(const SDL_KeyboardEvent& e);
void OnMouseButtonEvent(const SDL_MouseButtonEvent& e);
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
void OnMouseWheelEvent(const SDL_MouseWheelEvent& e);
void SceneEvents(const SDL_Event &event);
void GlobalEvents(const SDL_Event &event);
void ProcessEvents(const SDL_Event &event, bool ByPassSceneEvents);
void PollEvents();
void HandleEvents();


void SetViewPort(const std::unique_ptr<Camera>& current_camera);
void UpdateViewVolumeVertices();
void Update(float dt);
void Render(const std::unique_ptr<Camera>& current_camera, float dt);
void DrawAxes(float length = 10.0f);

int main(int argc, char** argv) {
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    // 設定 OpenGL Context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
#ifdef __linux__
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
#endif

    // 抗鋸齒
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    // 建立視窗
    const auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    window = SDL_CreateWindow(
        "OpenGL with SDL2!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, flags);
    if (window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(-1);
    }

    // 建立 OpenGL Context 與其他設定
    SDL_SetWindowMinimumSize(window, 400, 300);
    auto glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);

    // Setting the mouse mode
    SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);

    // 初始化 GLAD
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    // 輸出訊息
    std::cout << "OpenGL Version:        " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version:          " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Renderer:              " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor:                " << glGetString(GL_VENDOR) << std::endl;

    // 設定 gl
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 初始化 Imgui
    my_ui = std::make_unique<UI>(window, glContext);

    // 建立 Shader Program
    // my_shader = std::make_unique<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    alpha_shader = std::make_unique<Shader>("assets/shaders/alpha.vert", "assets/shaders/alpha.frag");
    lighting_shader = std::make_unique<Shader>("assets/shaders/lighting.vert", "assets/shaders/lighting.frag");

    // 建立 Model Matrix Stack
    model = std::make_unique<MatrixStack>();

    // 建立世界（相機、形狀等等都在這邊初始化）
    my_world.create();

    // 遊戲主迴圈
    while (!is_quit) {
        // 計算每 frame 的變化時間
        current_time = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        delta_time = current_time - last_time;
        last_time = current_time;

        // 事件處理
        HandleEvents();

        // 更新數據
        Update(delta_time);

        // 清除快取
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (current_screen_mode == ScreenMode::Divide) {
            // 分割畫面時
            for (unsigned int screen_mode = ScreenMode::OrthoX; screen_mode <= ScreenMode::Prspct; screen_mode++) {
                switch (screen_mode) {
                    case ScreenMode::OrthoX:
                        my_world.ortho_x_camera->viewport = { 0, window_height / 2, window_width / 2, window_height / 2 };
                        Render(my_world.ortho_x_camera, delta_time);
                        break;
                    case ScreenMode::OrthoY:
                        my_world.ortho_y_camera->viewport = {
                            window_width / 2, window_height / 2, window_width / 2, window_height / 2
                        };
                        Render(my_world.ortho_y_camera, delta_time);
                        break;
                    case ScreenMode::OrthoZ:
                        my_world.ortho_z_camera->viewport = { 0, 0, window_width / 2, window_height / 2 };
                        Render(my_world.ortho_z_camera, delta_time);
                        break;
                    default:
                        my_world.my_camera->viewport = { window_width / 2, 0, window_width / 2, window_height / 2 };
                        Render(my_world.my_camera, delta_time);
                        break;
                }
            }
        } else {
            // 單一畫面時
            switch (current_screen_mode) {
                case ScreenMode::OrthoX:
                    my_world.ortho_x_camera->viewport = { 0, 0, window_width, window_height };
                    Render(my_world.ortho_x_camera, delta_time);
                    break;
                case ScreenMode::OrthoY:
                    my_world.ortho_y_camera->viewport = { 0, 0, window_width, window_height };
                    Render(my_world.ortho_y_camera, delta_time);
                    break;
                case ScreenMode::OrthoZ:
                    my_world.ortho_z_camera->viewport = { 0, 0, window_width, window_height };
                    Render(my_world.ortho_z_camera, delta_time);
                    break;
                default:
                    my_world.my_camera->viewport = { 0, 0, window_width, window_height };
                    Render(my_world.my_camera, delta_time);
                    break;
            }
        }

        // 繪製 Imgui
        my_ui->Render();

        // 切換 Buffer
        SDL_GL_SwapWindow(window);
    }
    my_ui->Destroy();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void OnWindowEvent(const SDL_WindowEvent& e) {
    switch (e.event) {
        default:
            break;
    }
}

void OnKeyDownEvent(const SDL_KeyboardEvent& e) {
    switch (e.keysym.sym) {
        case SDLK_TAB:
            my_world.my_camera->ToggleMouseControl();
            break;
        case SDLK_x:
            my_world.drawaxes = !my_world.drawaxes;
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
    }
}

void OnMouseButtonEvent(const SDL_MouseButtonEvent& e) {
    switch (e.button) {
        case SDL_BUTTON_LEFT:
            break;
        case SDL_BUTTON_MIDDLE:
            break;
        case SDL_BUTTON_RIGHT:
            break;
    }
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e) {
    switch (e.state) {
        case SDL_BUTTON_LMASK:
            break;
        case SDL_BUTTON_RMASK:
            break;
        default:
            break;
    }
}

void OnMouseWheelEvent(const SDL_MouseWheelEvent& e) {
    my_world.my_camera->ProcessMouseScroll(e.y);
}

void SceneEvents(const SDL_Event &event) {
    switch (event.type) {
        case SDL_QUIT:
            is_quit = true;
            break;
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

void GlobalEvents(const SDL_Event &event) {
    switch (event.type) {
        case SDL_QUIT:
            is_quit = true;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_q) {
                if (event.key.keysym.mod & KMOD_CTRL) {
                    is_quit = true;
                }
            }
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                // 更新視窗長寬
                SDL_GetWindowSize(window, &window_width, &window_height);
            }
            break;
    }
}

void ProcessEvents(const SDL_Event &event, bool ByPassSceneEvents) {
    // 優先度大於 ImGui 的事件：離開程式
    GlobalEvents(event);

    if (ByPassSceneEvents) {
        return;
    }

    // 優先度小於 ImGui
    SceneEvents(event);
}

void PollEvents() {
    events.clear();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        events.push_back(event);
    }
}

void HandleEvents() {
    // 將所有事件拉出來到 vector 中
    PollEvents();

    // 透過迴圈一個一個去跑 vector 來跑事件處理
    for (auto it = std::cbegin(events); it != std::cend(events); it++) {
        // 先處理 ImGui 的事件
        my_ui->ProcessEvent(*it);

        // 再來處理 SDL 自己的事件
        ProcessEvents(*it, my_ui->WantCaptureEvent);
    }

    // 如果 ImGui 佔用了滑鼠與鍵盤時，就不會執行下面的事件控制
    if (my_ui->WantCaptureEvent) {
        return;
    }

    // 執行攝影機的控制，這邊事件觸發並不是透過 SDL_Event ，而是透過像是 SDL_GetKeyboardState() 和 SDL_GetRelativeMouseState() 去控制的。
    // Camera Event Handler
    my_world.my_camera->ProcessKeyboard();
    my_world.my_camera->ProcessMouseMovement();
}

void SetViewPort(const std::unique_ptr<Camera>& current_camera) {
    const auto& [x, y, w, h] = current_camera->viewport;
    glViewport(x, y, w, h);
}

void UpdateViewVolumeVertices() {
    // 取得攝影機投影矩陣的參數
    float t = my_world.my_camera->frustum.top;
    float b = my_world.my_camera->frustum.bottom;
    float r = my_world.my_camera->frustum.right;
    float l = my_world.my_camera->frustum.left;
    float n = my_world.my_camera->frustum.near;
    float f = my_world.my_camera->frustum.far;

    // 透視投影的話，別忘記遠平面的 top, bottom, right 和 left 都必須要乘上近平面與遠平面的比值
    float t_f = t * f / n;
    float b_f = -t_f;
    float r_f = r * f / n;
    float l_f = -r_f;

    // 建立成 4 維空間的（齊次坐標系）點，注意這些點的位置存在於眼睛坐標系
    glm::vec4 rtn = glm::vec4(r, t ,-n, 1.0f);
    glm::vec4 ltn = glm::vec4(l, t, -n, 1.0f);
    glm::vec4 lbn = glm::vec4(l, b, -n, 1.0f);
    glm::vec4 rbn = glm::vec4(r, b, -n, 1.0f);
    glm::vec4 rtf = glm::vec4(r_f, t_f ,-f, 1.0f);
    glm::vec4 ltf = glm::vec4(l_f, t_f, -f, 1.0f);
    glm::vec4 lbf = glm::vec4(l_f, b_f, -f, 1.0f);
    glm::vec4 rbf = glm::vec4(r_f, b_f, -f, 1.0f);

    // 求 view matrix 的反矩陣
    glm::mat4 current_view = glm::inverse(my_world.my_camera->View());

    // 儲存起來，之後會供給 ImGui 使用，詳情見 UI.cpp 的 WindowsRender()
    my_world.my_camera->nearPlaneVertex = {
        current_view * lbn,  current_view * rbn, current_view * rtn, current_view * ltn
    };
    my_world.my_camera->farPlaneVertex = {
        current_view * lbf, current_view * rbf, current_view * rtf, current_view * ltf
    };

    // 這邊近平面多一個往攝影機後 0.01 的距離，目的是這樣在第一人稱畫面時繪製 ViewVolume 才不會重疊在物品之【前】打架
    rtn = rtn + glm::vec4(0.0f, 0.0f, 0.01f, 0.0f);
    ltn = ltn + glm::vec4(0.0f, 0.0f, 0.01f, 0.0f);
    lbn = lbn + glm::vec4(0.0f, 0.0f, 0.01f, 0.0f);
    rbn = rbn + glm::vec4(0.0f, 0.0f, 0.01f, 0.0f);

    // 這邊遠平面多一個往攝影機前 0.2 的距離，目的是這樣在第一人稱畫面時繪製 ViewVolume 才不會重疊在物品之【後】打架
    rtf = rtf + glm::vec4(0.0f, 0.0f, -0.2f, 0.0f);
    ltf = ltf + glm::vec4(0.0f, 0.0f, -0.2f, 0.0f);
    lbf = lbf + glm::vec4(0.0f, 0.0f, -0.2f, 0.0f);
    rbf = rbf + glm::vec4(0.0f, 0.0f, -0.2f, 0.0f);

    // 將這八的頂點乘上 view matrix 反矩陣即可獲得在世界坐標系的位置
    rtn = current_view * rtn;
    ltn = current_view * ltn;
    lbn = current_view * lbn;
    rbn = current_view * rbn;
    rtf = current_view * rtf;
    ltf = current_view * ltf;
    lbf = current_view * lbf;
    rbf = current_view * rbf;

    // 更新頂點資料
    my_world.view_volume->vertices = {
        lbn.x, lbn.y, lbn.z,
        rbn.x, rbn.y, rbn.z,
        rtn.x, rtn.y, rtn.z,
        ltn.x, ltn.y, ltn.z,

        lbf.x, lbf.y, lbf.z,
        rbf.x, rbf.y, rbf.z,
        rtf.x, rtf.y, rtf.z,
        ltf.x, ltf.y, ltf.z,
    };

    // 別忘記要告訴 vbo 要重新讀取頂點資料
    my_world.view_volume->UpdateVertices();
}

void Update(float dt) {
    my_world.my_camera->Update(dt);
    my_world.ortho_x_camera->UpdateTargetPosition(my_world.my_camera->Position);
    my_world.ortho_y_camera->UpdateTargetPosition(my_world.my_camera->Position);
    my_world.ortho_z_camera->UpdateTargetPosition(my_world.my_camera->Position);

    UpdateViewVolumeVertices();

    my_world.earth->yaw += my_world.speed * dt;
}

void Render(const std::unique_ptr<Camera>& current_camera, float dt) {
    // 是否開啟 Back Face Culling
    if (my_world.culling) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }

    // 設定 Viewport
    SetViewPort(current_camera);

    // 設定 View 以及 Projection Matrix
    glm::mat4 view = current_camera->View();
    glm::mat4 projection = current_camera->Projection();

    lighting_shader->Use();
    lighting_shader->SetMat4("view", view);
    lighting_shader->SetMat4("projection", projection);

    // Setting Lighting
    lighting_shader->SetVec3("lights[0].ambient", my_world.my_light->Ambient);

    model->Push();
    model->Save(glm::translate(model->Top(), my_world.my_camera->Position));
    model->Save(glm::rotate(model->Top(), glm::radians(-my_world.my_camera->Yaw), glm::vec3(0.0, 1.0, 0.0)));
    model->Save(glm::rotate(model->Top(), glm::radians(my_world.my_camera->Pitch), glm::vec3(1.0, 0.0, 0.0)));
    model->Save(glm::rotate(model->Top(), glm::radians(my_world.my_camera->Roll), glm::vec3(0.0, 0.0, -1.0)));
    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, 0.0f, 1.0f)));
    // 繪製攝影機三軸
    DrawAxes(20.0f);
    model->Pop();

    model->Push();
    model->Save(glm::translate(model->Top(), my_world.my_camera->Position));
    model->Save(glm::rotate(model->Top(), glm::radians(-my_world.my_camera->Yaw), glm::vec3(0.0, 1.0, 0.0)));
    model->Save(glm::rotate(model->Top(), glm::radians(my_world.my_camera->Pitch), glm::vec3(1.0, 0.0, 0.0)));
    model->Save(glm::rotate(model->Top(), glm::radians(my_world.my_camera->Roll), glm::vec3(0.0, 0.0, -1.0)));
    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, 0.0f, 5.5f)));
    model->Save(glm::scale(model->Top(), glm::vec3(8.0f, 5.0f, 10.0f)));
    lighting_shader->SetVec3("objectColor", glm::vec3(0.2f));
    lighting_shader->SetMat4("model", model->Top());
    // 繪製攝影機本體
    my_world.my_cube->Draw();
    model->Pop();

    lighting_shader->Use();
    model->Push();
    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, -50.0f, 0.0f)));
    model->Save(glm::scale(model->Top(), glm::vec3(100.0f)));
    lighting_shader->SetVec3("objectColor", glm::vec3(42 / 255.0f, 219 / 255.0f, 89 / 255.0f));
    lighting_shader->SetMat4("model", model->Top());
    my_world.my_cube->Draw();
    model->Pop();

    model->Push();
    model->Push();
    model->Save(glm::translate(model->Top(), my_world.sun->position));
    model->Push();
    model->Save(glm::scale(model->Top(), my_world.sun->scale));
    lighting_shader->SetVec3("objectColor", glm::vec3(1.0f, 0.811764706f, 0.301960784f));
    lighting_shader->SetMat4("model", model->Top());
    my_world.my_sphere->Draw();
    model->Pop();
    DrawAxes();
    model->Pop();

    model->Push();
    model->Save(glm::rotate(model->Top(), glm::radians(my_world.earth->yaw / 365.0f), glm::vec3(0.0, 1.0, 0.0)));
    model->Save(glm::translate(model->Top(), my_world.earth->position));
    model->Save(glm::rotate(model->Top(), glm::radians(23.5f), glm::vec3(1.0, 0.0, 0.0)));
    model->Save(glm::rotate(model->Top(), glm::radians(my_world.earth->yaw), glm::vec3(0.0, 1.0, 0.0)));
    DrawAxes();

    model->Push();
    model->Save(glm::scale(model->Top(), my_world.earth->scale));
    lighting_shader->SetVec3("objectColor", glm::vec3(0.137254902f, 0.274509804f, 0.968627451f));
    lighting_shader->SetMat4("model", model->Top());
    my_world.my_sphere->Draw();
    model->Pop();

    model->Push();
    model->Save(glm::rotate(model->Top(), glm::radians(my_world.earth->yaw / 48.0f), glm::vec3(0.0, 1.0, 0.0)));
    model->Save(glm::translate(model->Top(), my_world.moon->position));
    DrawAxes();

    model->Save(glm::scale(model->Top(), my_world.moon->scale));
    lighting_shader->SetVec3("objectColor", glm::vec3(0.447058824f, 0.450980392f, 0.478431373f));
    lighting_shader->SetMat4("model", model->Top());
    my_world.my_sphere->Draw();

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
    my_world.view_volume->Draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    alpha_shader->SetVec4("objectColor", glm::vec4(0.889f, 0.889f, 0.889f, 0.3f));
    my_world.view_volume->Draw();
    model->Pop();
}

void DrawAxes(float length) {
    if (!my_world.drawaxes) {
        return;
    }

    model->Push();
    model->Save(glm::translate(model->Top(), glm::vec3(length / 2.0f, 0.0f, 0.0f)));
    model->Save(glm::scale(model->Top(), glm::vec3(length, 0.5f, 0.5f)));
    lighting_shader->SetVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
    lighting_shader->SetMat4("model", model->Top());
    my_world.my_cube->Draw();
    model->Pop();

    model->Push();
    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, length / 2.0f, 0.0f)));
    model->Save(glm::scale(model->Top(), glm::vec3(0.5f, length, 0.5f)));
    lighting_shader->SetVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f));
    lighting_shader->SetMat4("model", model->Top());
    my_world.my_cube->Draw();
    model->Pop();

    model->Push();
    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, 0.0f, length / 2.0f)));
    model->Save(glm::scale(model->Top(), glm::vec3(0.5f, 0.5f, length)));
    lighting_shader->SetVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));
    lighting_shader->SetMat4("model", model->Top());
    my_world.my_cube->Draw();
    model->Pop();
}