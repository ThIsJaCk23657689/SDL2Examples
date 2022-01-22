#include "UI.hpp"

#include <glad/glad.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <glm/gtc/type_ptr.hpp>

#include "World.hpp"

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

    // Camera Info Window Render
    if (Windows.CameraInfo.Visible) {
        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
        ImGui::Begin("Camera Info", &Windows.CameraInfo.Visible, Windows.CameraInfo.WindowFlags);
        if (ImGui::BeginTabBar("TabBar##Window_CameraInfo")) {
            if (ImGui::BeginTabItem("Settings")) {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                            1000.0f / ImGui::GetIO().Framerate,
                            ImGui::GetIO().Framerate);
                ImGui::Checkbox("Draw Axes ", &my_world.drawaxes);
                ImGui::Checkbox("Back Face Culling", &my_world.culling);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Camera")) {
                ImGui::Text("Position = (%.2f, %.2f, %.2f)",
                            my_world.my_camera->Position.x,
                            my_world.my_camera->Position.y,
                            my_world.my_camera->Position.z);
                ImGui::Text("Velocity = (%.2f, %.2f, %.2f)",
                            my_world.my_camera->Velocity.x,
                            my_world.my_camera->Velocity.y,
                            my_world.my_camera->Velocity.z);
                // ImGui::Text("Acceleration = (%.2f, %.2f, %.2f)", my_world.my_camera->Acceleration.x,
                // my_world.my_camera->Acceleration.y, my_world.my_camera->Acceleration.z);
                ImGui::Text("Front = (%.2f, %.2f, %.2f)",
                            my_world.my_camera->Front.x,
                            my_world.my_camera->Front.y,
                            my_world.my_camera->Front.z);
                ImGui::Text("Right = (%.2f, %.2f, %.2f)",
                            my_world.my_camera->Right.x,
                            my_world.my_camera->Right.y,
                            my_world.my_camera->Right.z);
                ImGui::Text("Up = (%.2f, %.2f, %.2f)",
                            my_world.my_camera->Up.x,
                            my_world.my_camera->Up.y,
                            my_world.my_camera->Up.z);
                ImGui::Text("World Up = (%.2f, %.2f, %.2f)",
                            my_world.my_camera->WorldUp.x,
                            my_world.my_camera->WorldUp.y,
                            my_world.my_camera->WorldUp.z);
                ImGui::Text("Pitch = %.2f deg", my_world.my_camera->Pitch);
                ImGui::Text("Yaw = %.2f deg", my_world.my_camera->Yaw);
                ImGui::Text("Roll = %.2f deg", my_world.my_camera->Roll);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Otho Camera")) {
                ImGui::Text("X Position = (%.2f, %.2f, %.2f)",
                            my_world.ortho_x_camera->Position.x,
                            my_world.ortho_x_camera->Position.y,
                            my_world.ortho_x_camera->Position.z);
                ImGui::Text("Y Position = (%.2f, %.2f, %.2f)",
                            my_world.ortho_y_camera->Position.x,
                            my_world.ortho_y_camera->Position.y,
                            my_world.ortho_y_camera->Position.z);
                ImGui::Text("Z Position = (%.2f, %.2f, %.2f)",
                            my_world.ortho_z_camera->Position.x,
                            my_world.ortho_z_camera->Position.y,
                            my_world.ortho_z_camera->Position.z);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Projection")) {
                ImGui::Text("Parameters");
                ImGui::BulletText("FoV = %.2f deg, Aspect = %.2f", my_world.my_camera->Zoom, my_world.my_camera->AspectRatio());
                ImGui::DragFloatRange2("Near / Far", &my_world.my_camera->frustum.near, &my_world.my_camera->frustum.far, 1.0f, 0.1f, 500.0f);
                ImGui::Spacing();

                if (ImGui::TreeNode("Projection Matrix")) {
                    glm::mat4 proj = my_world.my_camera->Projection();

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

                glm::vec4 temp_lbn = my_world.my_camera->nearPlaneVertex[0];
                glm::vec4 temp_rbn = my_world.my_camera->nearPlaneVertex[1];
                glm::vec4 temp_rtn = my_world.my_camera->nearPlaneVertex[2];
                glm::vec4 temp_ltn = my_world.my_camera->nearPlaneVertex[3];
                glm::vec4 temp_lbf = my_world.my_camera->farPlaneVertex[0];
                glm::vec4 temp_rbf = my_world.my_camera->farPlaneVertex[1];
                glm::vec4 temp_rtf = my_world.my_camera->farPlaneVertex[2];
                glm::vec4 temp_ltf = my_world.my_camera->farPlaneVertex[3];

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

                glm::mat4 view = my_world.my_camera->View();
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

                glm::mat4 proj = my_world.my_camera->Projection();
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

            if (ImGui::BeginTabItem("Lighting"))  {

                if (ImGui::TreeNode("Directional Light")) {
                    ImGui::Checkbox("Enable", &my_world.my_directional_light->Enable);
                    ImGui::SliderFloat3("Direction", glm::value_ptr(my_world.my_directional_light->Direction), -50.0f, 50.0f);
                    if (ImGui::ColorEdit3("Color##Direction", glm::value_ptr(my_world.my_directional_light->Color))) {
                        my_world.my_directional_light->UpdateColor();
                    }
                    ImGui::TreePop();
                }
                ImGui::Spacing();

                if (ImGui::TreeNode("Spotlight")) {
                    ImGui::Checkbox("Enable", &my_world.my_spotlight->Enable);
                    ImGui::SliderFloat3("Position", glm::value_ptr(my_world.my_spotlight->Position), -50.0f, 50.0f);
                    ImGui::SliderFloat3("Direction", glm::value_ptr(my_world.my_spotlight->Direction), -1.0f, 1.0f);
                    if (ImGui::ColorEdit3("Color", glm::value_ptr(my_world.my_spotlight->Color))) {
                        my_world.my_spotlight->UpdateColor();
                    }
                    ImGui::DragFloatRange2("Cutoff", &my_world.my_spotlight->Cutoff, &my_world.my_spotlight->OuterCutoff, 1.0f, 1.0f, 90.0f);
                    ImGui::TreePop();
                }
                ImGui::Spacing();

                for (int i = 0; i < my_world.my_point_lights.size(); ++i) {
                    if (ImGui::TreeNode(std::string("Point Lights " + std::to_string(i)).c_str())) {
                        ImGui::Checkbox("Enable", &my_world.my_point_lights[i]->Enable);
                        ImGui::SliderFloat3("Position", glm::value_ptr(my_world.my_point_lights[i]->Position), -50.0f, 50.0f);
                        if (ImGui::ColorEdit3("Color", glm::value_ptr(my_world.my_point_lights[i]->Color))) {
                            my_world.my_point_lights[i]->UpdateColor();
                        }
                        ImGui::TreePop();
                    }
                    ImGui::Spacing();
                }

                ImGui::SliderFloat("Shininess", &my_world.shininess, 1.0f, 512.0f);
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
        ImGui::Text("NTOU OpenGL Template - Camera");
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
                    "Copyright 2021, NTOU CSE 503 Authors\n");
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
                ImGui::Text("glm\nVersion 0.9.9.8\n");
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

#ifndef NDEBUG
    // Demo Window Render
    if (Windows.Demo.Visible) {
        ImGui::ShowDemoWindow(&Windows.Demo.Visible);
    }
#endif
}