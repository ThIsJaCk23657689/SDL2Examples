#include "Geometry/2D/Floor.hpp"

Floor::Floor(float _texture_scale) : texture_scale(_texture_scale) {
    Initialize();
}

void Floor::GenerateVertices() {
    vertices = {
            Vertex{ {-0.5, 0.0,  0.5}, {0, 1, 0}, {0, 0}},
            Vertex{ { 0.5, 0.0,  0.5}, {0, 1, 0}, {texture_scale, 0}},
            Vertex{ { 0.5, 0.0, -0.5}, {0, 1, 0}, {texture_scale, texture_scale}},
            Vertex{ {-0.5, 0.0, -0.5}, {0, 1, 0}, {0, texture_scale}},
    };

    indices = {
        0, 1, 2,
        0, 2, 3
    };
}