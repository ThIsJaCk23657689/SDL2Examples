#ifndef CUBE_HPP
#define CUBE_HPP

#include "Geometry/Geometry.hpp"

#include <glad/glad.h>
#include <vector>

struct Cube : public Geometry {
    Cube();

protected:
    void GenerateVertices() override;
};

#endif