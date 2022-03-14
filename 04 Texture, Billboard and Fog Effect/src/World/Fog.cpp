#include "World/Fog.hpp"

Fog::Fog(glm::vec3 col, float des, float f_s, float f_e, bool ena) :
    mode(FogMode::EXP), color(col), density(des), start(f_s), end(f_e), enable(ena) {
}