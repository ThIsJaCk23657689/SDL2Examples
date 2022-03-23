#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>

#include "Texture/Texture2D.hpp"

struct Material {
    Material();
    explicit Material(glm::vec3 col, float shin = 4.0f, Texture2D* tex = nullptr);
    explicit Material(Texture2D* tex);

    Texture2D* texture;
    bool emission_texture;

    glm::vec3 color;
    float shininess;
    bool mix;
};

#endif