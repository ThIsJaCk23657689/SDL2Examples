#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

enum CameraMovement : unsigned int {
    LEFT = 0,
    RIGHT = 1,
    DOWN = 2,
    UP = 3,
    FORWARD = 4,
    BACKWARD = 5,
};

struct Camera {
    Camera(glm::vec3 pos, bool is_prscpt = false);

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
    std::vector <glm::vec4> nearPlaneVertex;
    std::vector <glm::vec4> farPlaneVertex;

    float AspectRatio();
    glm::mat4 View();
    glm::mat4 Projection();
    glm::mat4 Orthogonal();
    glm::mat4 Perspective();

    void ProcessKeyboard();
    void ProcessMouseMovement(bool constrain = true);
    void ProcessMouseScroll(float yoffset);
    void ToggleMouseControl();
    void Update(float dt);
    void UpdateTargetPosition(glm::vec3 target);

    float Pitch, Yaw, Roll;
    glm::vec3 Position;
    glm::vec3 Velocity;
    glm::vec3 Acceleration;
    glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 Up;
    glm::vec3 Target;
    float Distance;
    float MoveSpeed;
    float MouseSensitivity;
    float Zoom;
    bool MouseControl;
    bool IsPerspective;
    bool FollowTarget;

private:
    void UpdateCameraVectors();
    void UpdateProjectionParameters();
};