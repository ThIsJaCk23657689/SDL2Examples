#include "World/Fog.hpp"

Fog::Fog(glm::vec3 col, float des, float f_s, float f_e, bool ena) :
    mode(FogMode::EXP), color(col), density(des), start(f_s), end(f_e), enable(ena) {
}

void Fog::Set(std::unique_ptr<Shader>& shader) {
    shader->SetVec3("fog.color", color);
    shader->SetInt("fog.mode", mode);
    shader->SetFloat("fog.density", density);
    shader->SetFloat("fog.start", start);
    shader->SetFloat("fog.end", end);
    shader->SetBool("fog.enable", enable);
}