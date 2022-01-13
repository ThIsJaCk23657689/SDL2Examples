#include "Geometry.hpp"

Geometry::~Geometry() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Geometry::Initialize() {
    GenerateVertices();
    BufferInitialize();
}

void Geometry::UpdateVertices() {
    // 更新只需要改 vbo 就好，ebo 完全不需要去動到
    // 因為幾乎每一幀數都會更新資料，建議將 glBuffData() 的最後一個參數改為 GL_DYNAMIC_DRAW
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<void*>(0));
    // glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Geometry::Draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}

void Geometry::BufferInitialize() {
    // 建立 VAO VBO EBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void Geometry::Clear() {
    vertices.clear();
    indices.clear();
}