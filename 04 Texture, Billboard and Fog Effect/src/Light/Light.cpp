#include "Light/Light.hpp"

// Directional Light
Light::Light(glm::vec4 dir, bool enb) :
    position(-dir),
    direction(dir),
    ambient(0.12f),
    diffuse(0.76f),
    specular(0.4f),
    color(1.0f),
    constant(1.0f),
    linear(0.0f),
    quadratic(0.0f),
    cutoff(0.0f),
    outer_cutoff(0.0f),
    enable(enb),
    caster(LightCaster::Directional){}

// Point Light
Light::Light(glm::vec3 pos, bool enb) :
    position(pos),
    direction(0.0f),
    ambient(0.12f),
    diffuse(0.76f),
    specular(0.4f),
    color(1.0f),
    constant(1.0f),
    linear(0.09f),
    quadratic(0.0032f),
    cutoff(0.0f),
    outer_cutoff(0.0f),
    enable(enb),
    caster(LightCaster::Point){}

// Spotlight
Light::Light(glm::vec3 pos, glm::vec3 dir, bool enb) :
    position(pos),
    direction(dir),
    ambient(0.12f),
    diffuse(0.76f),
    specular(0.4f),
    color(1.0f),
    constant(1.0f),
    linear(0.09f),
    quadratic(0.0032f),
    cutoff(12.0f),
    outer_cutoff(30.0f),
    enable(enb),
    caster(LightCaster::Spot){}

void Light::UpdateColor() {
    ambient = color * 0.12f;
    diffuse = color * 0.76f;
    specular = color * 0.4f;
}

void Light::UpdateColor(glm::vec3 col) {
    color = col;
    ambient = color * 0.12f;
    diffuse = color * 0.76f;
    specular = color * 0.4f;
}