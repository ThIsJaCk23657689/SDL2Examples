#include "UI.hpp"

#include <glad/glad.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <glm/gtc/type_ptr.hpp>

#include "World.hpp"

UI::UI(SDL_Window* window, SDL_GLContext glContext) : WindowHandler(window), GLContext(glContext) {
    create();
}

void UI::create() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(WindowHandler, GLContext);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.FrameBorderSize = 1.0f;
}

void UI::destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void UI::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(WindowHandler);
    ImGui::NewFrame();

    MenuBarRender();
    WindowsRender();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::MenuBarRender() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Edit##menubar-edit")) {
            ImGui::MenuItem("Planet Editor", nullptr, &Windows.PlanetEditor.Visible);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
#ifndef NDEBUG
            if (ImGui::MenuItem("Dear ImGui Demo", nullptr)) {
                Windows.Demo.Visible = true;
            }
#endif

            if (ImGui::MenuItem("About", nullptr)) {
                Windows.About.Visible = true;
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void UI::WindowsRender() {
    Windows.About.WindowFlags = ImGuiWindowFlags_NoResize;

    // Planet Editor Window Render
    if (Windows.PlanetEditor.Visible) {
        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
        ImGui::Begin("Planet Editor", &Windows.PlanetEditor.Visible, Windows.PlanetEditor.WindowFlags);
        if (ImGui::BeginTabBar("TabBar##Window_PlanetEditor")) {

            if (ImGui::BeginTabItem("Settings")) {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::Checkbox("Draw Axes ", &my_world.drawaxes);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Position")) {
                ImGui::SliderFloat3("Sun Position ", glm::value_ptr(my_world.sun->position), -50.0f, 50.0f);
                ImGui::SliderFloat3("Earth Position ", glm::value_ptr(my_world.earth->position), -50.0f, 50.0f);
                ImGui::SliderFloat3("Moon Position ", glm::value_ptr(my_world.moon->position), -50.0f, 50.0f);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Rotation")) {
                ImGui::SliderFloat("Speed ", &my_world.speed, 0.0f, 5000.0f);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Scale")) {
                ImGui::SliderFloat3("Sun ", glm::value_ptr(my_world.sun->scale), 1.0f, 10.0f);
                ImGui::SliderFloat3("Earth ", glm::value_ptr(my_world.earth->scale), 1.0f, 10.0f);
                ImGui::SliderFloat3("Moon ", glm::value_ptr(my_world.moon->scale), 1.0f, 10.0f);
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    // About Window Render
    if (Windows.About.Visible) {
        ImGui::Begin("About##Window_About", &Windows.About.Visible, Windows.About.WindowFlags);
        ImGui::SetWindowFontScale(1.2f);
        ImGui::Text("SDL2 Examples - 01 Model Matrices Stack and Sphere");
        ImGui::SetWindowFontScale(1.0f);
        if (ImGui::BeginTabBar("TabBar##Window_About")) {

            if (ImGui::BeginTabItem("About##About")) {
                ImGui::BeginChild("Child##AboutAbout", Windows.About.ChildSize, true);
                ImGui::TextWrapped("An OpenGL template program for students learning Computer Graphics.\n"
                                   "\n"
                                   "Developed at:\n"
                                   "National Taiwan Ocean University\n"
                                   "\n"
                                   "Copyright 2021, NTOU CSE 503 Authors\n");
                ImGui::EndChild();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Components##About")) {
                ImGui::BeginChild("Child##AboutComponents", Windows.About.ChildSize, true);
                ImGui::Text("SDL2\n"
                            "Version %d.%d.%d\n"
                            "https://www.libsdl.org/\n",
                            SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
                ImGui::Text(" ");
                ImGui::Separator();
                ImGui::Text("Dear ImGui\n"
                            "Version %s\n"
                            "https://github.com/ocornut/imgui\n",
                            IMGUI_VERSION);
                ImGui::EndChild();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Authors##About")) {
                ImGui::BeginChild("Child##AboutAuthors", Windows.About.ChildSize, true);
                ImGui::Text("ThIsJaCk <y23657689@gmail.com>\nKao Yü-chia <kogiokka.frms@gmail.com>\n");
                ImGui::EndChild();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::End();
    }

#ifndef NDEBUG
    // Demo Window Render
    if (Windows.Demo.Visible) {
        ImGui::ShowDemoWindow(&Windows.Demo.Visible);
    }
#endif
}