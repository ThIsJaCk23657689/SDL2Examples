#include "Light/Light.hpp"

// Directional Light (如果初始化 dir，會導致無效，因為現在光的 direction 是取自於 entities 的 front，而其 front 是靠 rotate 角度來計算的)
Light::Light(glm::vec4 dir, bool enb) :
    ambient(0.08f),
    diffuse(0.86f),
    specular(0.73f),
    color(1.0f),
    constant(1.0f),
    linear(0.0f),
    quadratic(0.0f),
    cutoff(0.0f),
    outer_cutoff(0.0f),
    enable(enb),
    caster(LightCaster::Directional),
    entity(Entity(glm::vec3(-dir.x, -dir.y, -dir.z))) {
    entity.material.color = color;
}

// Point Light
Light::Light(glm::vec3 pos, bool enb) :
    ambient(0.08f),
    diffuse(0.86f),
    specular(0.73f),
    color(1.0f),
    constant(0.0f),
    linear(0.0f),
    quadratic(1.0f),
    cutoff(0.0f),
    outer_cutoff(0.0f),
    enable(enb),
    caster(LightCaster::Point),
    entity(Entity(pos)) {
    entity.material.color = color;
}

// Spotlight (如果初始化 dir，會導致無效，因為現在光的 direction 是取自於 entities 的 front，而其 front 是靠 rotate 角度來計算的)
Light::Light(glm::vec3 pos, glm::vec3 dir, bool enb) :
    ambient(0.08f),
    diffuse(0.86f),
    specular(0.73f),
    color(1.0f),
    constant(1.0f),
    linear(0.09f),
    quadratic(0.0032f),
    cutoff(12.0f),
    outer_cutoff(30.0f),
    enable(enb),
    caster(LightCaster::Spot),
    entity(Entity(pos)) {
    entity.material.color = color;
}

void Light::HookCamera(const Camera* cam) {
    camera = cam;
    entity.position = camera->position;
    entity.rotate.x = camera->pitch;
    entity.rotate.y = camera->yaw;
    entity.front = camera->front;
}

void Light::Update(float dt) {
    if (camera != nullptr) {
        entity.position = camera->position;
        entity.rotate.x = camera->pitch;
        entity.rotate.y = camera->yaw;
        entity.front = camera->front;
    }
}

void Light::UpdateColor() {
    ambient = color * 0.08f;
    diffuse = color * 0.86f;
    specular = color * 0.73f;

    entity.material.color = color;
}

void Light::UpdateColor(glm::vec3 col) {
    color = col;
    UpdateColor();
}