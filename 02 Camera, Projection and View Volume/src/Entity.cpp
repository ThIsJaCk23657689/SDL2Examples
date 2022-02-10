#include "Entity.hpp"

Entity::Entity() :
    position(glm::vec3(0.0f)),
    velocity(glm::vec3(0.0f, 0.0f, 1.0f)),
    pitch(0.0f),
    yaw(0.0f),
    roll(0.0f),
    scale(glm::vec3(1.0f)),
    model(glm::mat4(1.0f)) {}

glm::mat4 Entity::GetModel(const glm::mat4& previous_matrix) {
    model = previous_matrix;
    model = glm::translate(model, position);

    model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, scale);

    return model;
}