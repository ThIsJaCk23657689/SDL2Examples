#include "Application.hpp"

#include <SDL.h>
#include <glad/glad.h>

#include "UI.hpp"
#include "Window.hpp"
#include "Util/Logger.hpp"

#include "State.hpp"

Application::Application(const Config& config) : my_config(config) {
    Initialize();
}

Application::~Application() {
    state.ui->Destroy();
    SDL_GL_DeleteContext(state.context);
    SDL_DestroyWindow(state.window->handler);
    SDL_Quit();
}

void Application::Initialize() {
    Logger::ShowMe();

    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        Logger::Message(LogLevel::SDLError, "Oops! Failed to initialize SDL2. :(");
        exit(-1);
    }
    Logger::Message(LogLevel::Info, "Initialize SDL2 successfully.");

    // Request an OpenGL 3.3 context and setting "Core" mode
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, my_config.opengl_major_version);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, my_config.opengl_minor_version);
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
#ifdef __linux__
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
#endif

    // Anti-aliasing
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    // Other Settings
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // Create a window
    state.window = std::make_unique<Window>();
    if (my_config.is_fullscreen) {
        const auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALLOW_HIGHDPI;
        state.window->handler = SDL_CreateWindow(my_config.title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                              0, 0, flags);
    } else {
        const auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
        state.window->handler = SDL_CreateWindow(my_config.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                              my_config.width, my_config.height, flags);
    }
    if (state.window->handler == nullptr) {
        Logger::Message(LogLevel::SDLError, "Failed to create SDL2 window: ");
        exit(-1);
    }
    SDL_SetWindowMinimumSize(state.window->handler, 400, 300);

    // Create a OpenGL context
    state.context = SDL_GL_CreateContext(state.window->handler);
    if (state.context == NULL) {
        Logger::Message(LogLevel::SDLError, "Failed to create OpenGL context with SDL2 window: ");
        exit(-1);
    }
    SDL_GL_MakeCurrent(state.window->handler, state.context);
    Logger::Message(LogLevel::Info, "Create OpenGL context successfully.");

    // This make our buffers swap synchronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    // Setting the mouse mode
    SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);

    // Initialize GLAD (Must behind the create window)
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        Logger::Message(LogLevel::Error, "Failed to initialize GLAD.");
        exit(-1);
    }
    Logger::Message(LogLevel::Info, "Initialize GLAD successfully.");

    // OpenGL basic settings
    SDL_GetWindowSize(state.window->handler, &state.window->width, &state.window->height);
    glViewport(0, 0, state.window->width, state.window->height);

    // 輸出訊息
    Logger::ShowGLInfo();

    state.ui = std::make_unique<UI>(state.window->handler, state.context);

    game = std::make_unique<Game>();
    game->current_screen_mode = my_config.screen_mode;
}

void Application::Run() {
    // 遊戲主迴圈
    while (!state.window->should_close) {

        // 計算每 frame 的變化時間
        current_time = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        delta_time = current_time - last_time;
        last_time = current_time;

        // 事件處理
        game->HandleEvents();

        // 更新數據
        game->Update(delta_time);

        // 呼叫 renderer 來清除快取
        game->RendererInit();

        if (game->current_screen_mode == ScreenMode::Divide) {
            // 分割畫面時
            for (unsigned int screen_mode = ScreenMode::OrthoX; screen_mode <= ScreenMode::Prspct; screen_mode++) {
                switch (screen_mode) {
                    case ScreenMode::OrthoX:
                        state.world->ortho_x_camera->viewport = { 0, state.window->height / 2, state.window->width / 2, state.window->height / 2 };
                        game->Render(state.world->ortho_x_camera, delta_time);
                        break;
                    case ScreenMode::OrthoY:
                        state.world->ortho_y_camera->viewport = {
                            state.window->width / 2, state.window->height / 2, state.window->width / 2, state.window->height / 2
                        };
                        game->Render(state.world->ortho_y_camera, delta_time);
                        break;
                    case ScreenMode::OrthoZ:
                        state.world->ortho_z_camera->viewport = { 0, 0, state.window->width / 2, state.window->height / 2 };
                        game->Render(state.world->ortho_z_camera, delta_time);
                        break;
                    default:
                        state.world->my_camera->viewport = { state.window->width / 2, 0, state.window->width / 2, state.window->height / 2 };
                        game->Render(state.world->my_camera, delta_time);
                        break;
                }
            }
        } else {
            // 單一畫面時
            switch (game->current_screen_mode) {
                case ScreenMode::OrthoX:
                    state.world->ortho_x_camera->viewport = { 0, 0, state.window->width, state.window->height };
                    game->Render(state.world->ortho_x_camera, delta_time);
                    break;
                case ScreenMode::OrthoY:
                    state.world->ortho_y_camera->viewport = { 0, 0, state.window->width, state.window->height };
                    game->Render(state.world->ortho_y_camera, delta_time);
                    break;
                case ScreenMode::OrthoZ:
                    state.world->ortho_z_camera->viewport = { 0, 0, state.window->width, state.window->height };
                    game->Render(state.world->ortho_z_camera, delta_time);
                    break;
                default:
                    state.world->my_camera->viewport = { 0, 0, state.window->width, state.window->height };
                    game->Render(state.world->my_camera, delta_time);
                    break;
            }
        }

        // 記得將 Viewport 切回正常大小，並且 Viewport settings
        state.world->my_camera->viewport = { 0, 0, state.window->width, state.window->height };
        state.world->my_camera->SetViewPort();

        // 執行高斯模糊，用於 Bloom 效果
        game->RenderGaussianBlur();

        // 繪製 Screen
        game->RenderScreen();

        // 繪製 Imgui
        state.ui->Render();

        // 切換 Buffer
        SDL_GL_SwapWindow(state.window->handler);
    }

    // Clean Up
    game->Destroy();
    Logger::Message(LogLevel::Info, "Good Bye :)");
}