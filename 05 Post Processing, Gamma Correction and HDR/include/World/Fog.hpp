#ifndef FOG_HPP
#define FOG_HPP

#include <glm/glm.hpp>

#include <memory>

#include "Shader/Shader.hpp"

enum FogMode : int {
    LINEAR = 0,
    EXP = 1,
    EXP2 = 2
};

struct Fog {
    FogMode mode;
    glm::vec3 color;
    float density;
    float gradient;
    float start;
    float end;
    bool enable;

    Fog(glm::vec3 col, float des = 0.15f, float grad = 1.0f, float f_s = 20.0f, float f_e = 80.0f, bool ena = true);

    void Set(std::unique_ptr<Shader>& shader);
};

#endif