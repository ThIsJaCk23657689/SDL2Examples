#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "Model/Vertex.hpp"

#include <glad/glad.h>
#include <vector>

constexpr float PI = 3.14159265359f;

enum Attributes : int {
    Position = 0,
    Normal = 1,
    Texture = 2
};

struct Geometry {
    GLuint vao, vbo, ebo;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    Geometry() = default;
    ~Geometry();

    void Initialize();
    void UpdateVertices();
    void Bind() const;
    void UnBind() const;
    void DrawOnly() const;
    void Draw() const;

protected:
    virtual void GenerateVertices() = 0;
    virtual void BufferInitialize();
    void Clear();
};
#endif