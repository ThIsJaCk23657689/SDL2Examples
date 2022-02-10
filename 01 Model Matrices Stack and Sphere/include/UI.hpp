#pragma once

#include <SDL.h>
#include <imgui.h>

struct UI {
    UI(SDL_Window* window, SDL_GLContext glContext);

    SDL_Window* WindowHandler;
    SDL_GLContext GLContext;
    bool WantCaptureEvent = false;

    struct Windows {
        struct PlanetEditor {
            bool Visible = false;
            int WindowFlags = 0;
        } PlanetEditor;
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

    void create();
    void destroy();
    void render();

private:
    void MenuBarRender();
    void WindowsRender();

};