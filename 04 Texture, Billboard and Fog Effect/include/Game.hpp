#ifndef GAME_HPP
#define GAME_HPP

#include <SDL.h>

#include <vector>
#include <memory>

#include "Config.hpp"
#include "Camera.hpp"
#include "Shader/Shader.hpp"
#include "Util/MatrixStack.hpp"

struct Game {
    Game();

    void Update(float dt);
    void Render(const std::unique_ptr<Camera>& current_camera, float dt);

    void HandleEvents();

    ScreenMode current_screen_mode;

private:
    void PollEvents();
    void ProcessEvents(const SDL_Event &event, bool ByPassSceneEvents);
    void GlobalEvents(const SDL_Event &event);
    void SceneEvents(const SDL_Event &event);

    void OnKeyDownEvent(const SDL_KeyboardEvent& e);
    void OnMouseButtonEvent(const SDL_MouseButtonEvent& e);
    void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
    void OnMouseWheelEvent(const SDL_MouseWheelEvent& e);
    void OnWindowEvent(const SDL_WindowEvent& e);

    // TODO:: Not support to be here
    void DrawAxes(float length = 10.0f);

    std::vector<SDL_Event> events = {};

    // TODO:: Make a Shader Manager to create
    std::unique_ptr<Shader> basic_shader = nullptr;
    std::unique_ptr<Shader> alpha_shader = nullptr;
    std::unique_ptr<Shader> lighting_shader = nullptr;
    std::unique_ptr<Shader> screen_shader = nullptr;

    std::unique_ptr<MatrixStack> model = nullptr;
};

#endif
