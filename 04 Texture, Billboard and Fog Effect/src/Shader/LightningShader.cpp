#include "Shader/LightningShader.hpp"

const std::string LightningShader::VERTEX_FILE = "assets/shaders/lightning.vert";
const std::string LightningShader::FRAGMENT_FILE = "assets/shaders/lightning.frag";

LightningShader::LightningShader() : Shader(VERTEX_FILE, FRAGMENT_FILE) {}

void LightningShader::SetLight(const std::unique_ptr<Light>& light, const int& light_index) {
    switch (light->caster) {
        case LightCaster::Directional:
            SetVec3("lights[" + std::to_string(light_index) + "].direction", light->entity.front);
            SetVec3("lights[" + std::to_string(light_index) + "].ambient", light->ambient);
            SetVec3("lights[" + std::to_string(light_index) + "].diffuse", light->diffuse);
            SetVec3("lights[" + std::to_string(light_index) + "].specular", light->specular);
            SetBool("lights[" + std::to_string(light_index) + "].enable", light->enable);
            SetInt( "lights[" + std::to_string(light_index) + "].caster", light->caster);
            break;
        case LightCaster::Point:
            SetVec3("lights[" + std::to_string(light_index) + "].position", light->entity.position);
            SetVec3("lights[" + std::to_string(light_index) + "].ambient", light->ambient);
            SetVec3("lights[" + std::to_string(light_index) + "].diffuse", light->diffuse);
            SetVec3("lights[" + std::to_string(light_index) + "].specular", light->specular);
            SetFloat("lights[" + std::to_string(light_index) + "].constant", light->constant);
            SetFloat("lights[" + std::to_string(light_index) + "].linear", light->linear);
            SetFloat("lights[" + std::to_string(light_index) + "].quadratic", light->quadratic);
            SetBool("lights[" + std::to_string(light_index) + "].enable", light->enable);
            SetInt("lights[" + std::to_string(light_index) + "].caster", light->caster);
            break;
        case LightCaster::Spot:
            SetVec3("lights[" + std::to_string(light_index) + "].position", light->entity.position);
            SetVec3("lights[" + std::to_string(light_index) + "].direction", light->entity.front);
            SetVec3("lights[" + std::to_string(light_index) + "].ambient", light->ambient);
            SetVec3("lights[" + std::to_string(light_index) + "].diffuse", light->diffuse);
            SetVec3("lights[" + std::to_string(light_index) + "].specular", light->specular);
            SetFloat("lights[" + std::to_string(light_index) + "].constant", light->constant);
            SetFloat("lights[" + std::to_string(light_index) + "].linear", light->linear);
            SetFloat("lights[" + std::to_string(light_index) + "].quadratic", light->quadratic);
            SetFloat("lights[" + std::to_string(light_index) + "].cutoff", glm::cos(glm::radians(light->cutoff)));
            SetFloat("lights[" + std::to_string(light_index) + "].outerCutoff", glm::cos(glm::radians(light->outer_cutoff)));
            SetBool("lights[" + std::to_string(light_index) + "].enable", light->enable);
            SetInt("lights[" + std::to_string(light_index) + "].caster", light->caster);
            break;
    }
}

void LightningShader::SetFog(const std::unique_ptr<Fog>& fog) {
    SetVec3("fog.color", fog->color);
    SetInt("fog.mode", fog->mode);
    SetFloat("fog.density", fog->density);
    SetFloat("fog.start", fog->start);
    SetFloat("fog.end", fog->end);
    SetBool("fog.enable", fog->enable);
}