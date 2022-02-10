#pragma once
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

    Position position;
    Normal normal;
};

static_assert(std::is_standard_layout_v<Vertex>);
static_assert(std::is_standard_layout_v<Vertex::Position>);
static_assert(std::is_standard_layout_v<Vertex::Normal>);