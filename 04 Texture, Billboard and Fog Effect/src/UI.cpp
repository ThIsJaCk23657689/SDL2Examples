#include "UI.hpp"

#include <glad/glad.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <glm/gtc/type_ptr.hpp>

#include "State.hpp"

UI::UI(SDL_Window* window, SDL_GLContext glContext) : WindowHandler(window), GLContext(glContext) {
    Create();
}

void UI::Create() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(WindowHandler, GLContext);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.FrameBorderSize = 1.0f;
}

void UI::Destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void UI::Render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(WindowHandler);
    ImGui::NewFrame();

    MenuBarRender();
    WindowsRender();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::ProcessEvent(const SDL_Event &event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    const ImGuiIO &io = ImGui::GetIO();
    WantCaptureEvent = io.WantCaptureKeyboard || io.WantCaptureMouse;
}

void UI::MenuBarRender() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Edit##menubar-edit")) {
            ImGui::MenuItem("Camera Info", nullptr, &Windows.CameraInfo.Visible);
            ImGui::MenuItem("Projection Info", nullptr, &Windows.ProjectionInfo.Visible);
            ImGui::MenuItem("Lightning Info", nullptr, &Windows.LightningInfo.Visible);
            ImGui::MenuItem("Settings", nullptr, &Windows.Settings.Visible);
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

    CameraInfoRender();
    ProjectionInfoRender();
    LightningInfoRender();
    SettingsRender();
    AboutRender();

#ifndef NDEBUG
    // Demo Window Render
    if (Windows.Demo.Visible) {
        ImGui::ShowDemoWindow(&Windows.Demo.Visible);
    }
#endif
}

void UI::CameraInfoRender() {
    // Camera Info Window Render
    if (Windows.CameraInfo.Visible) {
        ImGui::SetNextWindowSize(ImVec2(300, 250), ImGuiCond_Once);
        ImGui::Begin("Camera Info", &Windows.CameraInfo.Visible, Windows.CameraInfo.WindowFlags);

        if (ImGui::BeginTabBar("TabBar##Window_CameraInfo")) {

            if (ImGui::BeginTabItem("Camera")) {
                ImGui::Text("Position = (%.2f, %.2f, %.2f)",
                            state.world->my_camera->position.x,
                            state.world->my_camera->position.y,
                            state.world->my_camera->position.z);
                ImGui::Text("Velocity = (%.2f, %.2f, %.2f)",
                            state.world->my_camera->velocity.x,
                            state.world->my_camera->velocity.y,
                            state.world->my_camera->velocity.z);
                ImGui::Text("Front = (%.2f, %.2f, %.2f)",
                            state.world->my_camera->front.x,
                            state.world->my_camera->front.y,
                            state.world->my_camera->front.z);
                ImGui::Text("Right = (%.2f, %.2f, %.2f)",
                            state.world->my_camera->right.x,
                            state.world->my_camera->right.y,
                            state.world->my_camera->right.z);
                ImGui::Text("Up = (%.2f, %.2f, %.2f)",
                            state.world->my_camera->up.x,
                            state.world->my_camera->up.y,
                            state.world->my_camera->up.z);
                ImGui::Text("World Up = (%.2f, %.2f, %.2f)",
                            state.world->my_camera->world_up.x,
                            state.world->my_camera->world_up.y,
                            state.world->my_camera->world_up.z);
                ImGui::Text("Pitch = %.2f deg", state.world->my_camera->pitch);
                ImGui::Text("Yaw = %.2f deg", state.world->my_camera->yaw);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Otho Camera")) {
                ImGui::Text("X Position = (%.2f, %.2f, %.2f)",
                            state.world->ortho_x_camera->position.x,
                            state.world->ortho_x_camera->position.y,
                            state.world->ortho_x_camera->position.z);
                ImGui::Text("Y Position = (%.2f, %.2f, %.2f)",
                            state.world->ortho_y_camera->position.x,
                            state.world->ortho_y_camera->position.y,
                            state.world->ortho_y_camera->position.z);
                ImGui::Text("Z Position = (%.2f, %.2f, %.2f)",
                            state.world->ortho_z_camera->position.x,
                            state.world->ortho_z_camera->position.y,
                            state.world->ortho_z_camera->position.z);
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}

void UI::ProjectionInfoRender() {
    // Projection Window Render
    if (Windows.ProjectionInfo.Visible) {
        ImGui::SetNextWindowSize(ImVec2(450, 180), ImGuiCond_Once);
        ImGui::Begin("Projection Info", &Windows.ProjectionInfo.Visible, Windows.ProjectionInfo.WindowFlags);
        if (ImGui::BeginTabBar("TabBar##Window_ProjectionInfo")) {

            if (ImGui::BeginTabItem("Projection")) {
                ImGui::Text("Parameters:");
                ImGui::BulletText("FoV = %.2f deg, Aspect = %.2f", state.world->my_camera->zoom, state.world->my_camera->AspectRatio());
                ImGui::DragFloatRange2("Near / Far", &state.world->my_camera->frustum.near, &state.world->my_camera->frustum.far, 1.0f, 0.1f, 500.0f);
                ImGui::Spacing();

                if (ImGui::TreeNode("Projection Matrix")) {
                    glm::mat4 proj = state.world->my_camera->Projection();

                    ImGui::Columns(4, "mycolumns");
                    ImGui::Separator();
                    for (int i = 0; i < 4; i++) {
                        ImGui::Text("%.2f", proj[0][i]); ImGui::NextColumn();
                        ImGui::Text("%.2f", proj[1][i]); ImGui::NextColumn();
                        ImGui::Text("%.2f", proj[2][i]); ImGui::NextColumn();
                        ImGui::Text("%.2f", proj[3][i]); ImGui::NextColumn();
                        ImGui::Separator();
                    }
                    ImGui::Columns(1);

                    ImGui::TreePop();
                }
                ImGui::Spacing();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("ViewVolume")) {
                glm::vec4 temp_lbn = state.world->my_camera->near_plane_vertex[0];
                glm::vec4 temp_rbn = state.world->my_camera->near_plane_vertex[1];
                glm::vec4 temp_rtn = state.world->my_camera->near_plane_vertex[2];
                glm::vec4 temp_ltn = state.world->my_camera->near_plane_vertex[3];
                glm::vec4 temp_lbf = state.world->my_camera->far_plane_vertex[0];
                glm::vec4 temp_rbf = state.world->my_camera->far_plane_vertex[1];
                glm::vec4 temp_rtf = state.world->my_camera->far_plane_vertex[2];
                glm::vec4 temp_ltf = state.world->my_camera->far_plane_vertex[3];

                if (ImGui::TreeNode("View Volume Vertices (World Coordinate)")) {
                    ImGui::BulletText("left-bottom-near: (%.2f, %.2f, %.2f, %.2f)", temp_lbn.x, temp_lbn.y, temp_lbn.z, temp_lbn.w);
                    ImGui::BulletText("right-bottom-near: (%.2f, %.2f, %.2f, %.2f)", temp_rbn.x, temp_rbn.y, temp_rbn.z, temp_rbn.w);
                    ImGui::BulletText("right-top-near: (%.2f, %.2f, %.2f, %.2f)", temp_rtn.x, temp_rtn.y, temp_rtn.z, temp_rtn.w);
                    ImGui::BulletText("left-top-near: (%.2f, %.2f, %.2f, %.2f)", temp_ltn.x, temp_ltn.y, temp_ltn.z, temp_ltn.w);
                    ImGui::BulletText("left-bottom-far: (%.2f, %.2f, %.2f, %.2f)", temp_lbf.x, temp_lbf.y, temp_lbf.z, temp_lbf.w);
                    ImGui::BulletText("right-bottom-far: (%.2f, %.2f, %.2f, %.2f)", temp_rbf.x, temp_rbf.y, temp_rbf.z, temp_rbf.w);
                    ImGui::BulletText("right-top-far: (%.2f, %.2f, %.2f, %.2f)", temp_rtf.x, temp_rtf.y, temp_rtf.z, temp_rtf.w);
                    ImGui::BulletText("left-top-far: (%.2f, %.2f, %.2f, %.2f)", temp_ltf.x, temp_ltf.y, temp_ltf.z, temp_ltf.w);
                    ImGui::TreePop();
                }
                ImGui::Spacing();

                glm::mat4 view = state.world->my_camera->View();
                temp_lbn = view * temp_lbn;
                temp_rbn = view * temp_rbn;
                temp_rtn = view * temp_rtn;
                temp_ltn = view * temp_ltn;
                temp_lbf = view * temp_lbf;
                temp_rbf = view * temp_rbf;
                temp_rtf = view * temp_rtf;
                temp_ltf = view * temp_ltf;

                if (ImGui::TreeNode("View Volume Vertices (View Coordinate)")) {
                    ImGui::BulletText("left-bottom-near: (%.2f, %.2f, %.2f, %.2f)", temp_lbn.x, temp_lbn.y, temp_lbn.z, temp_lbn.w);
                    ImGui::BulletText("right-bottom-near: (%.2f, %.2f, %.2f, %.2f)", temp_rbn.x, temp_rbn.y, temp_rbn.z, temp_rbn.w);
                    ImGui::BulletText("right-top-near: (%.2f, %.2f, %.2f, %.2f)", temp_rtn.x, temp_rtn.y, temp_rtn.z, temp_rtn.w);
                    ImGui::BulletText("left-top-near: (%.2f, %.2f, %.2f, %.2f)", temp_ltn.x, temp_ltn.y, temp_ltn.z, temp_ltn.w);
                    ImGui::BulletText("left-bottom-far: (%.2f, %.2f, %.2f, %.2f)", temp_lbf.x, temp_lbf.y, temp_lbf.z, temp_lbf.w);
                    ImGui::BulletText("right-bottom-far: (%.2f, %.2f, %.2f, %.2f)", temp_rbf.x, temp_rbf.y, temp_rbf.z, temp_rbf.w);
                    ImGui::BulletText("right-top-far: (%.2f, %.2f, %.2f, %.2f)", temp_rtf.x, temp_rtf.y, temp_rtf.z, temp_rtf.w);
                    ImGui::BulletText("left-top-far: (%.2f, %.2f, %.2f, %.2f)", temp_ltf.x, temp_ltf.y, temp_ltf.z, temp_ltf.w);
                    ImGui::TreePop();
                }
                ImGui::Spacing();

                glm::mat4 proj = state.world->my_camera->Projection();
                temp_lbn = proj * temp_lbn;
                temp_rbn = proj * temp_rbn;
                temp_rtn = proj * temp_rtn;
                temp_ltn = proj * temp_ltn;
                temp_lbf = proj * temp_lbf;
                temp_rbf = proj * temp_rbf;
                temp_rtf = proj * temp_rtf;
                temp_ltf = proj * temp_ltf;

                if (ImGui::TreeNode("View Volume Vertices (Projection Coordinate)")) {
                    ImGui::BulletText("left-bottom-near: (%.2f, %.2f, %.2f, %.2f)", temp_lbn.x, temp_lbn.y, temp_lbn.z, temp_lbn.w);
                    ImGui::BulletText("right-bottom-near: (%.2f, %.2f, %.2f, %.2f)", temp_rbn.x, temp_rbn.y, temp_rbn.z, temp_rbn.w);
                    ImGui::BulletText("right-top-near: (%.2f, %.2f, %.2f, %.2f)", temp_rtn.x, temp_rtn.y, temp_rtn.z, temp_rtn.w);
                    ImGui::BulletText("left-top-near: (%.2f, %.2f, %.2f, %.2f)", temp_ltn.x, temp_ltn.y, temp_ltn.z, temp_ltn.w);
                    ImGui::BulletText("left-bottom-far: (%.2f, %.2f, %.2f, %.2f)", temp_lbf.x, temp_lbf.y, temp_lbf.z, temp_lbf.w);
                    ImGui::BulletText("right-bottom-far: (%.2f, %.2f, %.2f, %.2f)", temp_rbf.x, temp_rbf.y, temp_rbf.z, temp_rbf.w);
                    ImGui::BulletText("right-top-far: (%.2f, %.2f, %.2f, %.2f)", temp_rtf.x, temp_rtf.y, temp_rtf.z, temp_rtf.w);
                    ImGui::BulletText("left-top-far: (%.2f, %.2f, %.2f, %.2f)", temp_ltf.x, temp_ltf.y, temp_ltf.z, temp_ltf.w);
                    ImGui::TreePop();
                }
                ImGui::Spacing();

                if (ImGui::TreeNode("View Volume Vertices (Normalized Device Coordinate)")) {
                    ImGui::BulletText("left-bottom-near: (%.2f, %.2f, %.2f)", temp_lbn.x / temp_lbn.w, temp_lbn.y / temp_lbn.w, temp_lbn.z / temp_lbn.w);
                    ImGui::BulletText("right-bottom-near: (%.2f, %.2f, %.2f)", temp_rbn.x / temp_rbn.w, temp_rbn.y / temp_rbn.w, temp_rbn.z / temp_rbn.w);
                    ImGui::BulletText("right-top-near: (%.2f, %.2f, %.2f)", temp_rtn.x / temp_rtn.w, temp_rtn.y / temp_rtn.w, temp_rtn.z / temp_rtn.w);
                    ImGui::BulletText("left-top-near: (%.2f, %.2f, %.2f)", temp_ltn.x / temp_ltn.w, temp_ltn.y / temp_ltn.w, temp_ltn.z / temp_ltn.w);
                    ImGui::BulletText("left-bottom-far: (%.2f, %.2f, %.2f)", temp_lbf.x / temp_lbf.w, temp_lbf.y / temp_lbf.w, temp_lbf.z / temp_lbf.w);
                    ImGui::BulletText("right-bottom-far: (%.2f, %.2f, %.2f)", temp_rbf.x / temp_lbf.w, temp_rbf.y / temp_lbf.w, temp_rbf.z / temp_lbf.w);
                    ImGui::BulletText("right-top-far: (%.2f, %.2f, %.2f)", temp_rtf.x / temp_lbf.w, temp_rtf.y / temp_lbf.w, temp_rtf.z / temp_lbf.w);
                    ImGui::BulletText("left-top-far: (%.2f, %.2f, %.2f)", temp_ltf.x / temp_lbf.w, temp_ltf.y / temp_lbf.w, temp_ltf.z / temp_lbf.w);
                    ImGui::TreePop();
                }
                ImGui::Spacing();

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}

void UI::LightningInfoRender() {
    // Lightning Window Render
    if (Windows.LightningInfo.Visible) {
        ImGui::SetNextWindowSize(ImVec2(400, 180), ImGuiCond_Once);
        ImGui::Begin("Lightning Info", &Windows.LightningInfo.Visible, Windows.LightningInfo.WindowFlags);
        ImGui::SliderFloat("Shininess", &state.world->shininess, 1.0f, 512.0f);
        ImGui::Spacing();
        if (ImGui::BeginTabBar("TabBar##Window_LightningInfo")) {

            if (ImGui::BeginTabItem("Directional Light"))  {
                ImGui::Checkbox("Enable", &state.world->my_directional_light->enable);
                ImGui::SliderFloat3("Direction", glm::value_ptr(state.world->my_directional_light->direction), -50.0f, 50.0f);
                if (ImGui::ColorEdit3("Color##Direction", glm::value_ptr(state.world->my_directional_light->color))) {
                    state.world->my_directional_light->UpdateColor();
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Point Lights"))  {
                for (int i = 0; i < state.world->my_point_lights.size(); ++i) {
                    if (ImGui::TreeNode(std::string("Point Light " + std::to_string(i)).c_str())) {
                        ImGui::Checkbox("Enable", &state.world->my_point_lights[i]->enable);
                        ImGui::SliderFloat3("Position", glm::value_ptr(state.world->my_point_lights[i]->position), -50.0f, 50.0f);
                        if (ImGui::ColorEdit3("Color", glm::value_ptr(state.world->my_point_lights[i]->color))) {
                            state.world->my_point_lights[i]->UpdateColor();
                        }
                        ImGui::TreePop();
                    }
                    ImGui::Spacing();
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("SpotLight"))  {
                ImGui::Checkbox("Enable", &state.world->my_spotlight->enable);
                ImGui::SliderFloat3("Position", glm::value_ptr(state.world->my_spotlight->position), -50.0f, 50.0f);
                ImGui::SliderFloat3("Direction", glm::value_ptr(state.world->my_spotlight->direction), -1.0f, 1.0f);
                if (ImGui::ColorEdit3("Color", glm::value_ptr(state.world->my_spotlight->color))) {
                    state.world->my_spotlight->UpdateColor();
                }
                ImGui::DragFloatRange2("Cutoff", &state.world->my_spotlight->cutoff, &state.world->my_spotlight->outer_cutoff, 1.0f, 1.0f, 90.0f);
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}

void UI::SettingsRender() {
    // Settings Window Render
    if (Windows.Settings.Visible) {
        ImGui::SetNextWindowSize(ImVec2(380, 150), ImGuiCond_Once);
        ImGui::Begin("Settings", &Windows.Settings.Visible, Windows.Settings.WindowFlags);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::Checkbox("Draw Axes ", &state.world->draw_axes);
        ImGui::Checkbox("Back Face Culling", &state.world->culling);
        ImGui::End();
    }
}

void UI::AboutRender() {
    Windows.About.WindowFlags = ImGuiWindowFlags_NoResize;

    // About Window Render
    if (Windows.About.Visible) {
        ImGui::Begin("About##Window_About", &Windows.About.Visible, Windows.About.WindowFlags);
        ImGui::SetWindowFontScale(1.2f);
        ImGui::Text("SDL2 Examples - 03 Shading, Lightning and Material");
        ImGui::SetWindowFontScale(1.0f);
        if (ImGui::BeginTabBar("TabBar##Window_About")) {

            if (ImGui::BeginTabItem("About##About")) {
                ImGui::BeginChild("Child##AboutAbout", Windows.About.ChildSize, true);
                ImGui::TextWrapped(
                    "An OpenGL template program for students learning Computer Graphics.\n"
                    "\n"
                    "Developed at:\n"
                    "National Taiwan Ocean University\n"
                    "\n"
                    "Copyright 2022, NTOU CSE 503 Authors\n");
                ImGui::EndChild();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Components##About")) {
                ImGui::BeginChild("Child##AboutComponents", Windows.About.ChildSize, true);
                ImGui::Text(
                    "SDL2\n"
                    "Version %d.%d.%d\n"
                    "https://www.libsdl.org/\n",
                    SDL_MAJOR_VERSION,
                    SDL_MINOR_VERSION,
                    SDL_PATCHLEVEL);
                ImGui::Text(" ");
                ImGui::Separator();
                ImGui::Text("glad\nVersion 0.1.34\n");
                ImGui::Text(" ");
                ImGui::Separator();
                ImGui::Text("glm\nVersion %d.%d.%d.%d\n", GLM_VERSION_MAJOR, GLM_VERSION_MINOR, GLM_VERSION_PATCH, GLM_VERSION_REVISION);
                ImGui::Text(" ");
                ImGui::Separator();
                ImGui::Text(
                    "Dear ImGui\n"
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
}