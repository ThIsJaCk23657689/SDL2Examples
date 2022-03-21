#ifndef GAME_HPP
#define GAME_HPP

#include <SDL.h>

#include <vector>
#include <memory>

#include "Config.hpp"
#include "Camera.hpp"

#include "GL/FrameBuffer.hpp"
#include "GL/RenderBuffer.hpp"

#include "Renderer/MasterRenderer.hpp"
#include "Util/MatrixStack.hpp"

struct Game {
    Game();

    void RendererInit();
    void RenderScreen(const std::unique_ptr<Camera>& current_camera);
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

    void UpdateFramebuffer();

    std::vector<SDL_Event> events = {};

    // Renderer (Only this one)
    std::unique_ptr<MasterRenderer> master_renderer = nullptr;

    // Framebuffer and Renderbuffer (For Post Processing Purpose)
    std::unique_ptr<FrameBuffer> main_framebuffer = nullptr;
    std::unique_ptr<RenderBuffer> main_renderbuffer = nullptr;
};

#endif
