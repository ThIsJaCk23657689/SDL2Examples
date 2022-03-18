#include "Geometry/Geometry.hpp"

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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(Attributes::Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, position)));
    glVertexAttribPointer(Attributes::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, normal)));
    glVertexAttribPointer(Attributes::Texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, texture_coordinate)));
    glBindVertexArray(0);
}

void Geometry::Bind() const {
    glBindVertexArray(vao);
}

void Geometry::UnBind() const {
    glBindVertexArray(0);
}

void Geometry::DrawOnly() const {
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}

void Geometry::Draw() const {
    Bind();
    DrawOnly();
    UnBind();
}

void Geometry::BufferInitialize() {
    // 建立 VAO VBO EBO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(Attributes::Position);
    glEnableVertexAttribArray(Attributes::Normal);
    glEnableVertexAttribArray(Attributes::Texture);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(Attributes::Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, position)));
    glVertexAttribPointer(Attributes::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, normal)));
    glVertexAttribPointer(Attributes::Texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, texture_coordinate)));

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Geometry::Clear() {
    vertices.clear();
    indices.clear();
}