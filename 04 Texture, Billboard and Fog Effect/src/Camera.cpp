#include "Camera.hpp"

#include "SDL.h"
#include <glad/glad.h>

Camera::Camera(bool is_prscpt) :
    pitch(0.0f),
    yaw(0.0f),
    position({0, 0, 1}),
    velocity(glm::vec3(0.0f)),
    acceleration(glm::vec3(0.0f)),
    world_up(glm::vec3(0.0f, 1.0f, 0.0f)),
    front(glm::vec3(0.0, 0.0, -1.0)),
    right(glm::vec3(1.0, 0.0, 0.0)),
    up(glm::vec3(0.0, 1.0, 0.0)),
    target({0, 0, -1}),
    move_speed(150.0f),
    mouse_sensitivity(0.1f),
    zoom(45.0f),
    mouse_control(false),
    is_perspective(is_prscpt),
    follow_target(false)
{
    // UpdateCameraVectors();

    // Default Value
    frustum.near = 0.1f;
    frustum.far = 500.0f;
    UpdateProjectionParameters();
}

Camera::Camera(glm::vec3 pos, glm::vec3 target, bool is_prscpt) :
    pitch(0.0f),
    yaw(0.0f),
    position(pos),
    velocity(glm::vec3(0.0f)),
    acceleration(glm::vec3(0.0f)),
    world_up(glm::vec3(0.0f, 1.0f, 0.0f)),
    front(glm::vec3(0.0, 0.0, -1.0)),
    right(glm::vec3(1.0, 0.0, 0.0)),
    up(glm::vec3(0.0, 1.0, 0.0)),
    target(target),
    distance(0.0f),
    move_speed(150.0f),
    mouse_sensitivity(0.1f),
    zoom(45.0f),
    mouse_control(false),
    is_perspective(is_prscpt),
    follow_target(true)
{
    distance = glm::length(position - target);
    // UpdateCameraVectors();

    // Default Value
    frustum.near = 0.1f;
    frustum.far = 500.0f;
    UpdateProjectionParameters();
}

void Camera::UpdateCameraVectors() {
    // 更新攝影機的三軸方向向量：前、右、上，注意攝影機朝向負 z 軸
    glm::vec4 temp_front = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);

    if (follow_target) {
        // 代表此相機的旋轉是依據他的 target 的，所以先求出半徑（球形座標相機）
        glm::vec4 radius = glm::vec4(0.0f, 0.0f, distance, 1.0f);

        // 這邊 FollowTarget 我只專門用給那三個正交攝影機，設計專門跟隨著第一人稱視角的攝影機，所以說這邊我就不考慮 roll 了
        glm::mat4 rotateMatrix = glm::mat4(1.0f);
        rotateMatrix = glm::rotate(rotateMatrix, glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateMatrix = glm::rotate(rotateMatrix, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));

        // 攝影機會跟隨著目標移動
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(target.x, target.y, target.z));

        glm::vec4 pos = translation * rotateMatrix * radius;

        // 求出位置
        position = glm::vec3(pos.x, pos.y, pos.z);

        // Gram-Schmidt Orthogonalization 正交化求攝影機三軸
        front = glm::normalize(target - position);
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, right));
    } else {
        // 利用 rotation 矩陣來旋轉攝影機的【前】向量，pitch 為垂直旋轉、yaw 為水平旋轉，單位是角度
        // 因為 rotation 是逆時針旋轉(角度為正時)，而因為攝影機朝向負 z 軸，滑鼠的相對座標（螢幕坐標系）往右是正（向左轉），往左是負（向右轉）
        // 所以 Yaw 必須轉為負值，或者是將旋轉軸反過來也可以
        glm::mat4 rotateMatrix = glm::mat4(1.0f);
        rotateMatrix = glm::rotate(rotateMatrix, glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateMatrix = glm::rotate(rotateMatrix, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));

        // 記住攝影機的初始【前】向量永遠面向世界座標的 -z 軸。
        temp_front = rotateMatrix * temp_front;

        // Gram-Schmidt Orthogonalization 正交化求攝影機三軸
        front = glm::normalize(glm::vec3(temp_front.x, temp_front.y, temp_front.z));
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
    }
}

void Camera::UpdateTargetPosition(glm::vec3 target_pos) {
    target = target_pos;
    UpdateCameraVectors();
}

void Camera::HookEntity(const Entity &ent) {
    entity = &ent;
}

float Camera::AspectRatio() const {
    return static_cast<float>(viewport.width) / static_cast<float>(viewport.height);
}

glm::mat4 Camera::View() const {
    glm::mat4 view = glm::lookAt(position, position + front, world_up);
    return view;
}

void Camera::ProcessMouseScroll(float yoffset) {
    if (zoom >= 1.0f && zoom <= 90.0f) {
        zoom -= yoffset;
    }
    if (zoom < 1.0f) {
        zoom = 1.0f;
    }
    if (zoom > 90.0f) {
        zoom = 90.0f;
    }
}

void Camera::Update(float dt) {
    // 此處省略 up，固定都是 (0, 1, 0)。
    front = entity->front;
    right = entity->right;

    // 攝影機的位置會吃其跟隨 Entity 的位置、旋轉度數以及三軸，這後面加 5.5f 是怕攝影機本體會擋住畫面。
    position = entity->position + front * 5.5f;

    // 此處省略 roll (rotate.z)，固定都是 0 度。
    pitch = entity->rotate.x;
    yaw = entity->rotate.y;
}

glm::mat4 Camera::Projection() {
    UpdateProjectionParameters();
    if (is_perspective) {
        return Perspective();
    } else {
        return Orthogonal();
    }
}

glm::mat4 Camera::Orthogonal() {
    glm::mat4 proj = glm::ortho(frustum.left, frustum.right, frustum.bottom, frustum.top, frustum.near, frustum.far);
    return proj;
}

glm::mat4 Camera::Perspective() {
    glm::mat4 proj = glm::perspective(glm::radians(zoom), AspectRatio(), frustum.near, frustum.far);
    return proj;
}

void Camera::SetViewPort() {
    const auto& [x, y, w, h] = viewport;
    glViewport(x, y, w, h);
}

void Camera::UpdateProjectionParameters() {
    // 計算 ViewVolume 使用
    if (is_perspective) {
        frustum.top = tan(glm::radians(zoom / 2.0f)) * frustum.near;
        frustum.right = AspectRatio() * frustum.top;
        frustum.bottom = -frustum.top;
        frustum.left = -frustum.right;
    } else {
        frustum.top = tan(glm::radians(zoom / 2.0f)) * frustum.near * 1000.0f;
        frustum.right = AspectRatio() * frustum.top;
        frustum.bottom = -frustum.top;
        frustum.left = -frustum.right;
    }
}