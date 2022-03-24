#ifndef FLOOR_HPP
#define FLOOR_HPP

#include <memory>

#include "Geometry/Geometry.hpp"

struct Floor : Geometry {
    Floor(float _texture_scale = 1.0f);

protected:
    float texture_scale;
    void GenerateVertices() override;

};

#endif
