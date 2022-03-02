#include "World/World.hpp"

#include "Texture/TextureManager.hpp"

void World::create() {
    // Geometry Initialize
    my_cube = std::make_unique<Cube>();
    my_sphere = std::make_unique<Sphere>();
    view_volume = std::make_unique<Cube>();
    my_screen = std::make_unique<Screen>();

    // TODO:: Load Images and Create Textures
    TextureManager::Initialize();

    // Model Initialize
    planets = std::make_unique<Model>(my_sphere.get());
    rick_roll_cube_model = std::make_unique<Model>(my_cube.get(), &TextureManager::GetTexture2D("RickRoll"));

    // Entity Initialize
    rick_roll = std::make_unique<Entity>(rick_roll_cube_model.get(), glm::vec3(0.0f));
    sun = std::make_unique<Entity>(planets.get(), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(3.0f));
    earth = std::make_unique<Entity>(planets.get(), glm::vec3(30.0f, 0.0f, 0.0f));
    moon = std::make_unique<Entity>(
        planets.get(), glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    // Camera Initialize
    my_camera = std::make_unique<Camera>(glm::vec3(0.0f, 10.0f, 80.0f), true);
    ortho_x_camera = std::make_unique<Camera>(my_camera->position + glm::vec3(1.0f, 0.0f, 0.0f) * ortho_distance,
                                              my_camera->position);
    ortho_x_camera->yaw = -90.0f;
    ortho_y_camera = std::make_unique<Camera>(my_camera->position + glm::vec3(0.0f, 1.0f, 0.0f) * ortho_distance,
                                              my_camera->position);
    ortho_y_camera->pitch = -90.0f;
    ortho_y_camera->world_up = glm::vec3(0.0f, 0.0f, -1.0f);
    ortho_z_camera = std::make_unique<Camera>(my_camera->position + glm::vec3(0.0f, 0.0f, 1.0f) * ortho_distance,
                                              my_camera->position);

    // Light Initialize
    my_directional_light = std::make_unique<Light>(glm::vec4(0.0f, -1.0f, -3.0f, 0.0f), false);
    my_point_lights = {
        std::make_unique<Light>(glm::vec3(33.75f, 8.75f, -37.654f), true),
        std::make_unique<Light>(glm::vec3(-23.75f, 7.5f, 37.654f), true),
        std::make_unique<Light>(glm::vec3(-17.5f, 8.75f, -32.716f), true),
        std::make_unique<Light>(glm::vec3(32.5f, 7.5f, 23.309f), true),
        std::make_unique<Light>(glm::vec3(-42.5f, 12.5f, -5.556f), true),
        std::make_unique<Light>(glm::vec3(0.0f, 13.75f, 0.617f), true),
    };
    my_spotlight = std::make_unique<Light>(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, -0.5f, -1.0f), true);
    my_point_lights[0]->UpdateColor(glm::vec3(1.0f, 0.082f, 0.082f));
    my_point_lights[1]->UpdateColor(glm::vec3(0.082f, 0.082f, 1.0f));
    my_point_lights[2]->UpdateColor(glm::vec3(0.082f, 1.0f, 0.082f));
    my_point_lights[3]->UpdateColor(glm::vec3(0.82f, 0.0f, 1.0f));
    my_point_lights[4]->UpdateColor(glm::vec3(1.0f, 0.82f, 0.0f));
    my_point_lights[5]->UpdateColor(glm::vec3(165 / 255.0f, 177 / 255.0f, 1.0f));
}

void World::UpdateViewVolumeVertices() {
    // 取得攝影機投影矩陣的參數
    float t = my_camera->frustum.top;
    float b = my_camera->frustum.bottom;
    float r = my_camera->frustum.right;
    float l = my_camera->frustum.left;
    float n = my_camera->frustum.near;
    float f = my_camera->frustum.far;

    // 透視投影的話，別忘記遠平面的 top, bottom, right 和 left 都必須要乘上近平面與遠平面的比值
    float t_f = t * f / n;
    float b_f = -t_f;
    float r_f = r * f / n;
    float l_f = -r_f;

    // 建立成 4 維空間的（齊次坐標系）點，注意這些點的位置存在於眼睛坐標系
    glm::vec4 rtn = glm::vec4(r, t, -n, 1.0f);
    glm::vec4 ltn = glm::vec4(l, t, -n, 1.0f);
    glm::vec4 lbn = glm::vec4(l, b, -n, 1.0f);
    glm::vec4 rbn = glm::vec4(r, b, -n, 1.0f);
    glm::vec4 rtf = glm::vec4(r_f, t_f, -f, 1.0f);
    glm::vec4 ltf = glm::vec4(l_f, t_f, -f, 1.0f);
    glm::vec4 lbf = glm::vec4(l_f, b_f, -f, 1.0f);
    glm::vec4 rbf = glm::vec4(r_f, b_f, -f, 1.0f);

    // 求 view matrix 的反矩陣
    glm::mat4 current_view = glm::inverse(my_camera->View());

    // 儲存起來，之後會供給 ImGui 使用，詳情見 UI.cpp 的 WindowsRender()
    my_camera->near_plane_vertex = { current_view * lbn, current_view * rbn, current_view * rtn, current_view * ltn };
    my_camera->far_plane_vertex = { current_view * lbf, current_view * rbf, current_view * rtf, current_view * ltf };

    // 這邊近平面多一個往攝影機後 0.01 的距離，目的是這樣在第一人稱畫面時繪製 ViewVolume 才不會重疊在物品之【前】打架
    rtn = rtn + glm::vec4(0.0f, 0.0f, 0.01f, 0.0f);
    ltn = ltn + glm::vec4(0.0f, 0.0f, 0.01f, 0.0f);
    lbn = lbn + glm::vec4(0.0f, 0.0f, 0.01f, 0.0f);
    rbn = rbn + glm::vec4(0.0f, 0.0f, 0.01f, 0.0f);

    // 這邊遠平面多一個往攝影機前 0.2 的距離，目的是這樣在第一人稱畫面時繪製 ViewVolume 才不會重疊在物品之【後】打架
    rtf = rtf + glm::vec4(0.0f, 0.0f, -0.2f, 0.0f);
    ltf = ltf + glm::vec4(0.0f, 0.0f, -0.2f, 0.0f);
    lbf = lbf + glm::vec4(0.0f, 0.0f, -0.2f, 0.0f);
    rbf = rbf + glm::vec4(0.0f, 0.0f, -0.2f, 0.0f);

    // 將這八的頂點乘上 view matrix 反矩陣即可獲得在世界坐標系的位置
    rtn = current_view * rtn;
    ltn = current_view * ltn;
    lbn = current_view * lbn;
    rbn = current_view * rbn;
    rtf = current_view * rtf;
    ltf = current_view * ltf;
    lbf = current_view * lbf;
    rbf = current_view * rbf;

    // 更新頂點資料
    view_volume->vertices = {
        // Front
        Vertex { { lbn.x, lbn.y, lbn.z }, { 0.0f, 0.0f, 1.0f } },
        Vertex { { rbn.x, rbn.y, rbn.z }, { 0.0f, 0.0f, 1.0f } },
        Vertex { { rtn.x, rtn.y, rtn.z }, { 0.0f, 0.0f, 1.0f } },
        Vertex { { ltn.x, ltn.y, ltn.z }, { 0.0f, 0.0f, 1.0f } },

        // Left
        Vertex { { lbf.x, lbf.y, lbf.z }, { -1.0f, 0.0f, 0.0f } },
        Vertex { { lbn.x, lbn.y, lbn.z }, { -1.0f, 0.0f, 0.0f } },
        Vertex { { ltn.x, ltn.y, ltn.z }, { -1.0f, 0.0f, 0.0f } },
        Vertex { { ltf.x, ltf.y, ltf.z }, { -1.0f, 0.0f, 0.0f } },

        // Back
        Vertex { { lbf.x, lbf.y, lbf.z }, { 0.0f, 0.0f, -1.0f } },
        Vertex { { ltf.x, ltf.y, ltf.z }, { 0.0f, 0.0f, -1.0f } },
        Vertex { { rtf.x, rtf.y, rtf.z }, { 0.0f, 0.0f, -1.0f } },
        Vertex { { rbf.x, rbf.y, rbf.z }, { 0.0f, 0.0f, -1.0f } },

        // Right
        Vertex { { rbn.x, rbn.y, rbn.z }, { 1.0f, 0.0f, 0.0f } },
        Vertex { { rbf.x, rbf.y, rbf.z }, { 1.0f, 0.0f, 0.0f } },
        Vertex { { rtf.x, rtf.y, rtf.z }, { 1.0f, 0.0f, 0.0f } },
        Vertex { { rtn.x, rtn.y, rtn.z }, { 1.0f, 0.0f, 0.0f } },

        // Top
        Vertex { { ltn.x, ltn.y, ltn.z }, { 0.0f, 1.0f, 0.0f } },
        Vertex { { rtn.x, rtn.y, rtn.z }, { 0.0f, 1.0f, 0.0f } },
        Vertex { { rtf.x, rtf.y, rtf.z }, { 0.0f, 1.0f, 0.0f } },
        Vertex { { ltf.x, ltf.y, ltf.z }, { 0.0f, 1.0f, 0.0f } },

        // Bottom
        Vertex { { lbn.x, lbn.y, lbn.z }, { 0.0f, -1.0f, 0.0f } },
        Vertex { { lbf.x, lbf.y, lbf.z }, { 0.0f, -1.0f, 0.0f } },
        Vertex { { rbf.x, rbf.y, rbf.z }, { 0.0f, -1.0f, 0.0f } },
        Vertex { { rbn.x, rbn.y, rbn.z }, { 0.0f, -1.0f, 0.0f } },
    };

    // 別忘記要告訴 vbo 要重新讀取頂點資料
    view_volume->UpdateVertices();
}