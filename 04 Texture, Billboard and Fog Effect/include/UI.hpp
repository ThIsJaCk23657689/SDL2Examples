#ifndef UI_HPP
#define UI_HPP

#include <SDL.h>
#include <imgui.h>

#include <string>

struct UI {
    UI(SDL_Window* window, SDL_GLContext glContext);

    SDL_Window* WindowHandler;
    SDL_GLContext GLContext;
    bool WantCaptureEvent = false;

    struct Windows {
        struct CameraInfo {
            bool Visible = false;
            int WindowFlags = 0;
        } CameraInfo;
        struct ProjectionInfo {
            bool Visible = false;
            int WindowFlags = 0;
        } ProjectionInfo;
        struct LightningInfo {
            bool Visible = false;
            int WindowFlags = 0;
        } LightningInfo;
        struct FogInfo {
            bool Visible = false;
            int WindowFlags = 0;
        } FogInfo;
        struct Settings {
            bool Visible = false;
            int WindowFlags = 0;
        } Settings;
        struct About {
            bool Visible = false;
            int WindowFlags = 0;
            ImVec2 ChildSize = ImVec2(350, 300);
        } About;
#ifndef NDEBUG
        struct Demo {
            bool Visible = false;
        } Demo;
#endif
    } Windows;

    void Create();
    void Destroy();
    void Render();
    void ProcessEvent(const SDL_Event &event);

private:
    void MenuBarRender();
    void WindowsRender();

    void CameraInfoRender();
    void ProjectionInfoRender();
    void LightningInfoRender();
    void FogInfoRender();
    void SettingsRender();
    void AboutRender();
};

#endif