#pragma once

#include "Geometry/Geometry.hpp"

#include <glad/glad.h>
#include <vector>

struct Cube : public Geometry {
    Cube();

protected:
    void GenerateVertices() override;
};