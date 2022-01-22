#pragma once
#include <glm/glm.hpp>

enum LightCaster : int {
    Directional = 0,
    Point,
    Spot,
};

struct Light {
    glm::vec3 Position;
    glm::vec3 Direction;

    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    glm::vec3 Color;

    float Constant;
    float Linear;
    float Quadratic;

    float Cutoff;
    float OuterCutoff;

    bool Enable;
    LightCaster Caster;

    // Directional Light
    Light(glm::vec4 dir = glm::vec4(0.0, -1.0, -1.0f, 0.0f), bool enable = true);

    // Point Light
    Light(glm::vec3 pos = glm::vec3(0.0, 0.0, 5.0f), bool enable = true);

    // Spotlight
    Light(glm::vec3 pos = glm::vec3(0.0, 0.0, 5.0f), glm::vec3 dir = glm::vec3(0.0, 0.0, -1.0f), bool enable = true);

    void UpdateColor();
    void UpdateColor(glm::vec3 color);
};