#include "World/Entity.hpp"

Entity::Entity(Model * mod, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca) :
    model(mod),
    position(pos),
    velocity({0, 0, 0}),
    acceleration({0, 0, 0}),
    rotate(rot),
    scale(sca) {}

glm::mat4 Entity::GetModelMatrix(const glm::mat4& previous_matrix) {
    glm::mat4 model_matrix = previous_matrix;
    model_matrix = glm::translate(model_matrix, position);

    model_matrix = glm::rotate(model_matrix, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model_matrix = glm::scale(model_matrix, scale);

    return model_matrix;
}

void Entity::Update(float dt) {

}

void Entity::UpdatePosition(glm::vec3 delta, float dt) {
    position += delta * dt;
}

void Entity::UpdateRotation(glm::vec3 delta, float dt) {
    rotate += delta * dt;
}