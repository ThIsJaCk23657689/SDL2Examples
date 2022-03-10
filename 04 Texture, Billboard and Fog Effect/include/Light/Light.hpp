#ifndef LIGHT_HPP
#define LIGHT_HPP
#include <glm/glm.hpp>

#include "World/Entity.hpp"
#include "Camera.hpp"

enum LightCaster : int {
    Directional = 0,
    Point,
    Spot,
};

struct Light {
    // glm::vec3 position;
    // glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 color;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outer_cutoff;

    bool enable;
    LightCaster caster;

    Entity entity;
    const Camera* camera;

    // Directional Light
    Light(glm::vec4 dir = glm::vec4(0.0, -1.0, -1.0f, 0.0f), bool enb = true);

    // Point Light
    Light(glm::vec3 pos = glm::vec3(0.0, 0.0, 5.0f), bool enb = true);

    // Spotlight
    Light(glm::vec3 pos = glm::vec3(0.0, 0.0, 5.0f), glm::vec3 dir = glm::vec3(0.0, 0.0, -1.0f), bool enb = true);

    void HookCamera(const Camera* cam);
    void Update(float dt);
    void UpdateColor();
    void UpdateColor(glm::vec3 color);
};
#endif