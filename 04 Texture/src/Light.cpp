#include "Light.hpp"

// Directional Light
Light::Light(glm::vec4 dir, bool enable) :
    Position(-dir),
    Direction(dir),
    Ambient(0.12f),
    Diffuse(0.76f),
    Specular(0.4f),
    Color(1.0f),
    Constant(1.0f),
    Linear(0.0f),
    Quadratic(0.0f),
    Cutoff(0.0f),
    OuterCutoff(0.0f),
    Enable(enable),
    Caster(LightCaster::Directional){}

// Point Light
Light::Light(glm::vec3 pos, bool enable) :
    Position(pos),
    Direction(0.0f),
    Ambient(0.12f),
    Diffuse(0.76f),
    Specular(0.4f),
    Color(1.0f),
    Constant(1.0f),
    Linear(0.09f),
    Quadratic(0.0032f),
    Cutoff(0.0f),
    OuterCutoff(0.0f),
    Enable(enable),
    Caster(LightCaster::Point){}

// Spotlight
Light::Light(glm::vec3 pos, glm::vec3 dir, bool enable) :
    Position(pos),
    Direction(dir),
    Ambient(0.12f),
    Diffuse(0.76f),
    Specular(0.4f),
    Color(1.0f),
    Constant(1.0f),
    Linear(0.09f),
    Quadratic(0.0032f),
    Cutoff(12.0f),
    OuterCutoff(30.0f),
    Enable(enable),
    Caster(LightCaster::Spot){}

void Light::UpdateColor() {
    Ambient = Color * 0.12f;
    Diffuse = Color * 0.76f;
    Specular = Color * 0.4f;
}

void Light::UpdateColor(glm::vec3 color) {
    Color = color;
    Ambient = Color * 0.12f;
    Diffuse = Color * 0.76f;
    Specular = Color * 0.4f;
}