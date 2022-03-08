#ifndef GAME_HPP
#define GAME_HPP

#include <SDL.h>

#include <vector>
#include <memory>

#include "Config.hpp"

#include "Shader/BasicShader.hpp"
#include "Shader/LightningShader.hpp"
#include "Shader/AlphaShader.hpp"

#include "Renderer/MasterRenderer.hpp"
#include "Renderer/EntitiesRenderer.hpp"

#include "Camera.hpp"
#include "Util/MatrixStack.hpp"

struct Game {
    Game();

    void RendererInit();
    void Update(float dt);
    void Render(const std::unique_ptr<Camera>& current_camera, float dt);
    void Destroy();

    void HandleEvents();

    ScreenMode current_screen_mode = ScreenMode::Prspct;

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
    std::unique_ptr<BasicShader> basic_shader = nullptr;
    std::unique_ptr<AlphaShader> alpha_shader = nullptr;
    std::unique_ptr<LightningShader> lighting_shader = nullptr;
    std::unique_ptr<Shader> screen_shader = nullptr;

    // Renderer
    std::unique_ptr<MasterRenderer> master_renderer = nullptr;
    std::unique_ptr<EntitiesRenderer> entities_renderer = nullptr;

    std::unique_ptr<MatrixStack> model = nullptr;
};

#endif
