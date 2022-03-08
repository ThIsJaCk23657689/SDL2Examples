#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model/Model.hpp"

struct Entity {
    Model *model;

    // Model Transformations and basic information
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 rotate; // store in degrees (pitch, yaw, roll)
    glm::vec3 scale;

    Entity(Model * const mod, glm::vec3 pos = {0, 0, 0}, glm::vec3 rot = {0, 0, 0}, glm::vec3 sca = {1, 1, 1});
    glm::mat4 GetModelMatrix(const glm::mat4& previous_matrix = glm::mat4(1.0f));

    void Update(float dt);
    void UpdatePosition(glm::vec3 delta, float dt);
    void UpdateRotation(glm::vec3 delta, float dt);
};