#include "Screen.hpp"

Screen::Screen() {
    Initialize();
}

Screen::~Screen() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Screen::Initialize() {
    GenerateVertices();
    BufferInitialize();
}

void Screen::Draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Screen::BufferInitialize() {
    // 建立 VAO VBO EBO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(Attributes::Position);
    glEnableVertexAttribArray(Attributes::Texture);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(Attributes::Position, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<const void*>(0));
    glVertexAttribPointer(Attributes::Texture, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<const void*>(2 * sizeof(GLfloat)));

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Screen::Clear() {
    vertices.clear();
    indices.clear();
}

void Screen::GenerateVertices() {
    vertices = {
        -1.0, -1.0, 0.0, 0.0,
         1.0, -1.0, 1.0, 0.0,
         1.0,  1.0, 1.0, 1.0,
        -1.0,  1.0, 0.0, 1.0,
    };

    indices = {
        0, 1, 2,
        0, 2, 3,
    };
}