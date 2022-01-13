#pragma once

#include "Geometry.hpp"

#include <glad/glad.h>
#include <vector>

struct Cube : public Geometry {
    Cube();

protected:
    void GenerateVertices() override;

};