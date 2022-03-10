#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture/Texture2D.hpp"
#include "Material/Material.hpp"

struct Entity {
    Material material;

    // Model Transformations and basic information
    glm::vec3 position;
    glm::vec3 rotate; // store in degrees (pitch, yaw, roll)
    glm::vec3 scale;

    // The 3-axes
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 front;
    glm::vec3 world_up;

    // For movement
    glm::vec3 velocity;
    glm::vec3 acceleration;
    bool movable;

    Entity(glm::vec3 pos = {0, 0, 0}, glm::vec3 rot = {0, 0, 0}, glm::vec3 sca = {1, 1, 1});
    Entity(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, Texture2D* tex);
    Entity(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, glm::vec3 color);
    glm::mat4 GetModelMatrix(const glm::mat4& previous_matrix = glm::mat4(1.0f)) const;

    void Update(float dt);
    void UpdatePosition(glm::vec3 delta, float dt);
    void UpdateRotation(glm::vec3 delta, float dt);

private:
    void UpdateAxes();

};

#endif