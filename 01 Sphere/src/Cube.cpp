#include "Cube.hpp"

Cube::Cube() {
    Initialize();
}

void Cube::GenerateVertices() {
    vertices = {
        // Position
        -0.5f,  -0.5f,   0.5f,
         0.5f,  -0.5f,   0.5f,
         0.5f,   0.5f,   0.5f,
        -0.5f,   0.5f,   0.5f,

        -0.5f,  -0.5f,  -0.5f,
         0.5f,  -0.5f,  -0.5f,
         0.5f,   0.5f,  -0.5f,
        -0.5f,   0.5f,  -0.5f,
    };

    indices = {
        // Front
        0, 1, 2,
        0, 2, 3,

        // Left
        4, 0, 3,
        4, 3, 7,

        // Back
        5, 4, 7,
        5, 7, 6,

        // Right
        1, 5, 6,
        1, 6, 2,

        // Top
        3, 2, 6,
        3, 6, 7,

        // Bottom
        1, 0, 4,
        1, 4, 5,
    };
}