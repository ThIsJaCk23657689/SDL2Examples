#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "Geometry/Geometry.hpp"
#include <glad/glad.h>
#include <vector>

struct Screen {
    GLuint vao, vbo, ebo;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    Screen();
    ~Screen();

    void Initialize();
    void Draw() const;

protected:
    void GenerateVertices();
    void BufferInitialize();
    void Clear();

};
#endif