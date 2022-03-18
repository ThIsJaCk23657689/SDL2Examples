#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "World/Entity.hpp"

// 攝影機本身不會移動，只會跟隨有綁定的 Entity
// Entity 會移動，但鍵盤滑鼠是控制 Player 裡面的 Entity (就跟 Light 包含 Entity 一樣)

enum CameraMovement : unsigned int {
    LEFT = 0,
    RIGHT = 1,
    DOWN = 2,
    UP = 3,
    FORWARD = 4,
    BACKWARD = 5,
};

struct Camera {
    explicit Camera(bool is_prscpt = false);

    Camera(glm::vec3 pos, glm::vec3 target, bool is_prscpt = false);

    struct Viewport {
        int x;
        int y;
        int width;
        int height;
    } viewport;

    struct Proj {
        float left;
        float right;
        float bottom;
        float top;
        float near;
        float far;
    } frustum;
    std::vector <glm::vec4> near_plane_vertex;
    std::vector <glm::vec4> far_plane_vertex;

    float AspectRatio() const;
    glm::mat4 View() const;
    glm::mat4 Projection();
    glm::mat4 Orthogonal();
    glm::mat4 Perspective();
    void SetViewPort();


    void ProcessMouseScroll(float yoffset);
    void Update(float dt);
    void UpdateTargetPosition(glm::vec3 target_pos);

    void HookEntity(const Entity &ent);
    const Entity* entity;

    float pitch, yaw;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 target;
    float distance;
    float move_speed;
    float mouse_sensitivity;
    float zoom;
    bool mouse_control;
    bool is_perspective;
    bool follow_target;

private:
    void UpdateCameraVectors();
    void UpdateProjectionParameters();
};

#endif