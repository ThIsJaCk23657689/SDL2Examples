#pragma once

#include <glad/glad.h>
#include <vector>

constexpr float PI = 3.14159265359f;

struct Geometry {
    GLuint vao, vbo, ebo;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    Geometry() = default;
    ~Geometry();

    void Initialize();
    void Draw();

protected:
    virtual void GenerateVertices() = 0;
    virtual void BufferInitialize();
    void Clear();
};