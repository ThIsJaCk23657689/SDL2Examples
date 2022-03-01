#include "Geometry/3D/Cube.hpp"

Cube::Cube() {
    Initialize();
}

void Cube::GenerateVertices() {
    vertices = {
        // Front
        Vertex { {-0.5f, -0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }, { 0.0, 0.0 } },
        Vertex { { 0.5f, -0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }, { 1.0, 0.0 } },
        Vertex { { 0.5f,  0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }, { 1.0, 1.0 } },
        Vertex { {-0.5f,  0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }, { 0.0, 1.0 } },

        // Left
        Vertex { {-0.5f, -0.5f, -0.5f }, {-1.0f,  0.0f,  0.0f }, { 0.0, 0.0 } },
        Vertex { {-0.5f, -0.5f,  0.5f }, {-1.0f,  0.0f,  0.0f }, { 1.0, 0.0 } },
        Vertex { {-0.5f,  0.5f,  0.5f }, {-1.0f,  0.0f,  0.0f }, { 1.0, 1.0 } },
        Vertex { {-0.5f,  0.5f, -0.5f }, {-1.0f,  0.0f,  0.0f }, { 0.0, 1.0 } },

        // Back
        Vertex { {-0.5f, -0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 1.0, 0.0 } },
        Vertex { {-0.5f,  0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 1.0, 1.0 } },
        Vertex { { 0.5f,  0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 0.0, 1.0 } },
        Vertex { { 0.5f, -0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 0.0, 0.0 } },

        // Right
        Vertex { { 0.5f, -0.5f,  0.5f }, { 1.0f,  0.0f,  0.0f }, { 0.0, 0.0 } },
        Vertex { { 0.5f, -0.5f, -0.5f }, { 1.0f,  0.0f,  0.0f }, { 1.0, 0.0 } },
        Vertex { { 0.5f,  0.5f, -0.5f }, { 1.0f,  0.0f,  0.0f }, { 1.0, 1.0 } },
        Vertex { { 0.5f,  0.5f,  0.5f }, { 1.0f,  0.0f,  0.0f }, { 0.0, 1.0 } },

        // Top
        Vertex { {-0.5f,  0.5f,  0.5f }, { 0.0f,  1.0f,  0.0f }, { 0.0, 0.0 } },
        Vertex { { 0.5f,  0.5f,  0.5f }, { 0.0f,  1.0f,  0.0f }, { 1.0, 0.0 } },
        Vertex { { 0.5f,  0.5f, -0.5f }, { 0.0f,  1.0f,  0.0f }, { 1.0, 1.0 } },
        Vertex { {-0.5f,  0.5f, -0.5f }, { 0.0f,  1.0f,  0.0f }, { 0.0, 1.0 } },

        // Bottom
        Vertex { {-0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f,  0.0f }, { 1.0, 0.0 } },
        Vertex { {-0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f,  0.0f }, { 1.0, 1.0 } },
        Vertex { { 0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f,  0.0f }, { 0.0, 1.0 } },
        Vertex { { 0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f,  0.0f }, { 0.0, 0.0 } },
    };

    indices = {
        // Front
        0, 1, 2,
        0, 2, 3,

        // Left
        4, 5, 6,
        4, 6, 7,

        // Back
        8, 9, 10,
        8, 10, 11,

        // Right
        12, 13, 14,
        12, 14, 15,

        // Top
        16, 17, 18,
        16, 18, 19,

        // Bottom
        20, 21, 22,
        20, 22, 23,
    };
}