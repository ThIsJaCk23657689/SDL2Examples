#include "Light.hpp"

Light::Light(glm::vec3 pos, glm::vec3 color) :
    Entity(pos) {
    Ambient = color * 0.12f;
    Diffuse = color * 0.7f;
    Specular = glm::vec3(0.4f, 0.4f, 0.4f);
}

Light::Light(glm::vec3 pos, glm::vec4 color) :
    Entity(pos) {
    Ambient = glm::vec3(color.r, color.g, color.b) * 0.12f * color.a;
    Diffuse = glm::vec3(color.r, color.g, color.b) * 0.7f * color.a;
    Specular = glm::vec3(0.4f, 0.4f, 0.4f) * color.a;;
}