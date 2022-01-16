#pragma once
#include "Entity.hpp"

#include <glm/glm.hpp>

struct Light : Entity {
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    float Intensity;

    Light(glm::vec3 pos = glm::vec3(0.0, 0.0, 5.0f), glm::vec3 color = glm::vec3(1.0f));
    Light(glm::vec3 pos, glm::vec4 color);
};