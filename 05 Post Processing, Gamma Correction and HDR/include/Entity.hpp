#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Entity {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    float pitch;
    float yaw;
    float roll;

    glm::vec3 scale;
    glm::mat4 model;

    Entity();
    Entity(glm::vec3 pos);
    glm::mat4 GetModel(const glm::mat4& previous_matrix);
};