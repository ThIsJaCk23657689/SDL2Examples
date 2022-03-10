#include "World/Entity.hpp"

Entity::Entity(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca) :
    position(pos),
    velocity({0, 0, 0}),
    acceleration({0, 0, 0}),
    rotate(rot),
    scale(sca),
    material(Material()), right({1, 0, 0}), up({0, 1, 0}), front({0, 0, -1}), world_up({0, 1, 0}), movable(false) {
        UpdateAxes();
}

Entity::Entity(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, Texture2D* tex) :
    position(pos),
    velocity({0, 0, 0}),
    acceleration({0, 0, 0}),
    rotate(rot),
    scale(sca),
    material(Material(tex)), right({1, 0, 0}), up({0, 1, 0}), front({0, 0, -1}), world_up({0, 1, 0}), movable(false) {
    UpdateAxes();
}

Entity::Entity(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, glm::vec3 color) :
    position(pos),
    velocity({0, 0, 0}),
    acceleration({0, 0, 0}),
    rotate(rot),
    scale(sca),
    material(Material(color)), right({1, 0, 0}), up({0, 1, 0}), front({0, 0, -1}), world_up({0, 1, 0}), movable(false) {
    UpdateAxes();
}

glm::mat4 Entity::GetModelMatrix(const glm::mat4& previous_matrix) const {
    glm::mat4 model_matrix = previous_matrix;
    model_matrix = glm::translate(model_matrix, position);

    model_matrix = glm::rotate(model_matrix, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model_matrix = glm::scale(model_matrix, scale);

    return model_matrix;
}

void Entity::Update(float dt) {
    UpdateAxes();

    if (movable) {
        velocity += acceleration * dt;
        position += velocity * dt;

        acceleration = glm::vec3(0.0f);
        velocity *= 0.95f;
    }
}

void Entity::UpdatePosition(glm::vec3 delta, float dt) {
    position += delta * dt;
}

void Entity::UpdateRotation(glm::vec3 delta, float dt) {
    rotate += delta * dt;
}

void Entity::UpdateAxes() {
    // 更新物件的三軸方向向量：前、右、上，注意物件是朝向 -z 軸
    glm::vec4 temp_front = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);

    // 利用 rotation 矩陣來旋轉物件的【前】向量，pitch(rotate.x) 為垂直旋轉、yaw(rotate.y) 為水平旋轉，單位是角度
    glm::mat4 rotateMatrix = glm::mat4(1.0f);
    rotateMatrix = glm::rotate(rotateMatrix, glm::radians(-rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotate.z), glm::vec3(1.0f, 0.0f, -1.0f));

    // 記住物件的初始【前】向量永遠面向世界座標的 -z 軸。
    temp_front = rotateMatrix * temp_front;

    // 將 front 當作旋轉軸，並且 Roll 為度數，去旋轉 WorldUp 即可。
    world_up = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.z), glm::vec3(front.x, front.y, front.z)) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    // Gram-Schmidt Orthogonalization 正交化求攝影機三軸（注意因為 front 是指向正 z 軸所以算法會不太一樣）
    front = glm::normalize(glm::vec3(temp_front.x, temp_front.y, temp_front.z));
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}