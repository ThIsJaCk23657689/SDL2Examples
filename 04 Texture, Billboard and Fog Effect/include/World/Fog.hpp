#ifndef FOG_HPP
#define FOG_HPP

#include <glm/glm.hpp>

enum FogMode : int {
    LINEAR = 0,
    EXP = 1,
    EXP2 = 2
};

struct Fog {
    FogMode mode;
    glm::vec3 color;
    float density;
    float start;
    float end;
    bool enable;

    Fog(glm::vec3 col, float des = 0.15f, float f_s = 20.0f, float f_e = 80.0f, bool ena = true);
};

#endif