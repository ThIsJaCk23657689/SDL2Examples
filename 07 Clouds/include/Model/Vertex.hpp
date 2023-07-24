#ifndef VERTEX_HPP
#define VERTEX_HPP
#include <type_traits>

struct Vertex {
    struct Position {
        float x, y, z;
        float* ptr() { return reinterpret_cast<float*>(this); }
    };

    struct Normal {
        float x, y, z;
        float* ptr() { return reinterpret_cast<float*>(this); }
    };

    struct TexCoord {
        float u, v;
        float* ptr() { return reinterpret_cast<float*>(this); }
    };

    Position position;
    Normal normal;
    TexCoord texture_coordinate;
};

struct VolumeVertex {
    struct Position {
        float x, y, z;
        float* ptr() { return reinterpret_cast<float*>(this); }
    };

    struct TexCoord3D {
        float u, v, w;
        float* ptr() { return reinterpret_cast<float*>(this); }
    };

    Position position;
    TexCoord3D texture_coordinate;
};


static_assert(std::is_standard_layout_v<Vertex>);
static_assert(std::is_standard_layout_v<Vertex::Position>);
static_assert(std::is_standard_layout_v<Vertex::Normal>);
static_assert(std::is_standard_layout_v<Vertex::TexCoord>);
#endif