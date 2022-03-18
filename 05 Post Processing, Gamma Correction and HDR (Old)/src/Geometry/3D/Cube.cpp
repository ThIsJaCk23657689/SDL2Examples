#include "Geometry/3D/Cube.hpp"

Cube::Cube() {
    Initialize();
}

void Cube::GenerateVertices() {
    vertices = {
        // Front
        Vertex { {-0.5f, -0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }, { 0.0, 0.0 } },
        Vertex { { 0.5f, -0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }, { 1.0, 0.0 } },
        Vertex { { 0.5f,  0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }, { 1.0, 1.0 } },
        Vertex { {-0.5f,  0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }, { 0.0, 1.0 } },

        // Left
        Vertex { {-0.5f, -0.5f, -0.5f }, {-1.0f,  0.0f,  0.0f }, { 0.0, 0.0 } },
        Vertex { {-0.5f, -0.5f,  0.5f }, {-1.0f,  0.0f,  0.0f }, { 1.0, 0.0 } },
        Vertex { {-0.5f,  0.5f,  0.5f }, {-1.0f,  0.0f,  0.0f }, { 1.0, 1.0 } },
        Vertex { {-0.5f,  0.5f, -0.5f }, {-1.0f,  0.0f,  0.0f }, { 0.0, 1.0 } },

        // Back
        Vertex { {-0.5f, -0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 1.0, 0.0 } },
        Vertex { {-0.5f,  0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 1.0, 1.0 } },
        Vertex { { 0.5f,  0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 0.0, 1.0 } },
        Vertex { { 0.5f, -0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 0.0, 0.0 } },

        // Right
        Vertex { { 0.5f, -0.5f,  0.5f }, { 1.0f,  0.0f,  0.0f }, { 0.0, 0.0 } },
        Vertex { { 0.5f, -0.5f, -0.5f }, { 1.0f,  0.0f,  0.0f }, { 1.0, 0.0 } },
        Vertex { { 0.5f,  0.5f, -0.5f }, { 1.0f,  0.0f,  0.0f }, { 1.0, 1.0 } },
        Vertex { { 0.5f,  0.5f,  0.5f }, { 1.0f,  0.0f,  0.0f }, { 0.0, 1.0 } },

        // Top
        Vertex { {-0.5f,  0.5f,  0.5f }, { 0.0f,  1.0f,  0.0f }, { 0.0, 0.0 } },
        Vertex { { 0.5f,  0.5f,  0.5f }, { 0.0f,  1.0f,  0.0f }, { 1.0, 0.0 } },
        Vertex { { 0.5f,  0.5f, -0.5f }, { 0.0f,  1.0f,  0.0f }, { 1.0, 1.0 } },
        Vertex { {-0.5f,  0.5f, -0.5f }, { 0.0f,  1.0f,  0.0f }, { 0.0, 1.0 } },

        // Bottom
        Vertex { {-0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f,  0.0f }, { 1.0, 0.0 } },
        Vertex { {-0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f,  0.0f }, { 1.0, 1.0 } },
        Vertex { { 0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f,  0.0f }, { 0.0, 1.0 } },
        Vertex { { 0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f,  0.0f }, { 0.0, 0.0 } },
    };

    indices = {
        // Front
        0, 1, 2,
        0, 2, 3,

        // Left
        4, 5, 6,
        4, 6, 7,

        // Back
        8, 9, 10,
        8, 10, 11,

        // Right
        12, 13, 14,
        12, 14, 15,

        // Top
        16, 17, 18,
        16, 18, 19,

        // Bottom
        20, 21, 22,
        20, 22, 23,
    };
}

void Cube::UpdateViewVolume(std::unique_ptr<Camera>& camera) {
    // 取得攝影機投影矩陣的參數
    float t = camera->frustum.top;
    float b = camera->frustum.bottom;
    float r = camera->frustum.right;
    float l = camera->frustum.left;
    float n = camera->frustum.near;
    float f = camera->frustum.far;

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
    glm::mat4 current_view = glm::inverse(camera->View());

    // 儲存起來，之後會供給 ImGui 使用，詳情見 UI.cpp 的 WindowsRender()
    camera->near_plane_vertex = { current_view * lbn, current_view * rbn, current_view * rtn, current_view * ltn };
    camera->far_plane_vertex = { current_view * lbf, current_view * rbf, current_view * rtf, current_view * ltf };

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
    vertices = {
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
    UpdateVertices();
}