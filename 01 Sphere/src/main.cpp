#include <glad/glad.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>

#include "UI.hpp"
#include "Shader.hpp"
#include "MatrixStack.hpp"
#include "World.hpp"

bool is_quit = false;
float current_time = 0.0f;
float delta_time  = 0.0f;
float last_time  = 0.0f;

// 視窗屬性
int window_width = 800;
int window_height = 600;
SDL_Window* window = nullptr;

std::unique_ptr<UI> my_ui = nullptr;
std::unique_ptr<Shader> my_shader = nullptr;
std::unique_ptr<MatrixStack> model = nullptr;

void HandleEvents();
void OnKeyDownEvent(SDL_KeyboardEvent key_event);
void OnWindowEvent(SDL_WindowEvent window_event);
void Update(float dt);
void Render(float dt);
void DrawAxes();

int main(int argc, char **argv) {

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

    // 建立視窗
    const auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    window = SDL_CreateWindow("OpenGL with SDL2!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, flags);
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

    // 初始化 GLAD
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    // 輸出訊息
    std::cout << "OpenGL Version:        " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version:          " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Renderer:              " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor:                " << glGetString(GL_VENDOR) << std::endl;

    // 設定 gl
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // 初始化 Imgui
    my_ui = std::make_unique<UI>(window, glContext);

    // 建立 Shader Program
    my_shader = std::make_unique<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    // 建立 Model Matrix Stack
    model = std::make_unique<MatrixStack>();

    // 建立世界
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

        // 繪圖
        Render(delta_time);

        // 繪製 Imgui
        my_ui->render();

        // 切換 Buffer
        SDL_GL_SwapWindow(window);
    }
    my_ui->destroy();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

// 處理 SDL 事件
void HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                is_quit = true;
                break;
            case SDL_KEYDOWN:
                OnKeyDownEvent(event.key);
                break;
            case SDL_WINDOWEVENT:
                OnWindowEvent(event.window);
                break;
        }
    }
}

void OnKeyDownEvent(SDL_KeyboardEvent key_event) {
    switch (key_event.keysym.sym) {
        case SDLK_q:
            if (KMOD_CTRL & key_event.keysym.mod) {
                is_quit = true;
            }
            break;
    }
}

void OnWindowEvent(SDL_WindowEvent window_event) {
    if (window_event.event == SDL_WINDOWEVENT_RESIZED) {
        SDL_GetWindowSize(window, &window_width, &window_height);
        glViewport(0, 0, window_width, window_height);
    }
}

void Update(float dt) {
    my_world.earth->yaw += my_world.speed * dt;
}

void Render(float dt) {
    my_shader->Use();
    my_shader->SetMat4("projection", glm::perspective(45.0f, static_cast<float>(window_width) / static_cast<float>(window_height), 0.1f, 500.0f));
    my_shader->SetMat4("view", glm::lookAt(glm::vec3(0.0f, 10.0f, 80.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    model->Push();
        model->Push();
            model->Save(glm::translate(model->Top(), my_world.sun->position));
            model->Push();
                model->Save(glm::scale(model->Top(), my_world.sun->scale));
                my_shader->SetVec3("objectColor", glm::vec3(1.0f, 0.811764706f, 0.301960784f));
                my_shader->SetMat4("model", model->Top());
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
                my_shader->SetVec3("objectColor", glm::vec3(0.137254902f, 0.274509804f, 0.968627451f));
                my_shader->SetMat4("model", model->Top());
                my_world.my_sphere->Draw();
            model->Pop();

            model->Push();
                model->Save(glm::rotate(model->Top(), glm::radians(my_world.earth->yaw / 48.0f), glm::vec3(0.0, 1.0, 0.0)));
                model->Save(glm::translate(model->Top(), my_world.moon->position));
                DrawAxes();

                model->Save(glm::scale(model->Top(), my_world.moon->scale));
                my_shader->SetVec3("objectColor", glm::vec3(0.447058824f, 0.450980392f, 0.478431373f));
                my_shader->SetMat4("model", model->Top());
                my_world.my_sphere->Draw();

            model->Pop();
        model->Pop();
    model->Pop();
}

void DrawAxes() {
    if (!my_world.drawaxes) {
        return;
    }

    model->Push();
        model->Save(glm::translate(model->Top(), glm::vec3(5.0f, 0.0f, 0.0f)));
        model->Save(glm::scale(model->Top(), glm::vec3(10.0f, 0.5f, 0.5f)));
        my_shader->SetVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
        my_shader->SetMat4("model", model->Top());
        my_world.my_cube->Draw();
    model->Pop();

    model->Push();
        model->Save(glm::translate(model->Top(), glm::vec3(0.0f, 5.0f, 0.0f)));
        model->Save(glm::scale(model->Top(), glm::vec3(0.5f, 10.0f, 0.5f)));
        my_shader->SetVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f));
        my_shader->SetMat4("model", model->Top());
        my_world.my_cube->Draw();
    model->Pop();

    model->Push();
        model->Save(glm::translate(model->Top(), glm::vec3(0.0f, 0.0f, 5.0f)));
        model->Save(glm::scale(model->Top(), glm::vec3(0.5f, 0.5f, 10.0f)));
        my_shader->SetVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));
        my_shader->SetMat4("model", model->Top());
        my_world.my_cube->Draw();
    model->Pop();
}