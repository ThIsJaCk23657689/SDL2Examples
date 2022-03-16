#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>

#include <vector>
#include <memory>

#include "Geometry/Geometry.hpp"
#include "Camera.hpp"

struct Cube : public Geometry {
    Cube();
    void UpdateViewVolume(std::unique_ptr<Camera>& camera);

protected:
    void GenerateVertices() override;
};

#endif