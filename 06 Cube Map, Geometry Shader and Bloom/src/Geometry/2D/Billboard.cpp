#include "Geometry/2D/Billboard.hpp"

Billboard::Billboard() {
    Initialize();
}

void Billboard::GenerateVertices() {
    vertices = {
        Vertex { {-0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0, 0.0 } },
        Vertex { { 0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0, 0.0 } },
        Vertex { { 0.5f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0, 1.0 } },
        Vertex { {-0.5f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0, 1.0 } },
    };

    indices = {
        0, 1, 2,
        0, 2, 3,
    };
}

void Billboard::UpdateBillboard(std::unique_ptr<Camera>& camera, BillboardMode mode) {
    glm::vec3 c = -camera->front;
    glm::vec3 b = (mode == BillboardMode::FIXEDY)? glm::vec3(0.0f, 1.0f, 0.0f) : camera->up;
    glm::vec3 a = glm::normalize(glm::cross(b, c));

    // 只計算旋轉，不及算位置以及縮放！！
    glm::vec3 v0 = glm::vec3(0.0, 0.0, 0.0) - 0.5f * a;
    glm::vec3 v1 = glm::vec3(0.0, 0.0, 0.0) + 0.5f * a;
    glm::vec3 v2 = v1 + 1.0f * b;
    glm::vec3 v3 = v0 + 1.0f * b;

    vertices = {
        {{v0.x, v0.y, v0.z}, {-camera->front.x, -camera->front.y, -camera->front.z}, {0.0, 0.0}},
        {{v1.x, v1.y, v1.z}, {-camera->front.x, -camera->front.y, -camera->front.z}, {1.0, 0.0}},
        {{v2.x, v2.y, v2.z}, {-camera->front.x, -camera->front.y, -camera->front.z}, {1.0, 1.0}},
        {{v3.x, v3.y, v3.z}, {-camera->front.x, -camera->front.y, -camera->front.z}, {0.0, 1.0}},
    };

    UpdateVertices();
}