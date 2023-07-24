#include "Model/Cloud.hpp"
#include "Model/Vertex.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "PerlinNoise.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "Geometry/Geometry.hpp"
#include "Texture/Texture3D.hpp"
#include <algorithm>
#include <random>

Cloud::Cloud(const glm::vec3& pos, const glm::vec3& size, const glm::vec3& color)
    : m_Position(pos)
    , m_Size(size)
    , m_Color(color)
{
    Initialize();
}

Cloud::~Cloud() {
    Destroy();
}

void Cloud::Initialize() {
    GenerateVertices();
    BufferInitialize();

    CreatePerlinNoise();
    GenerateTextureData();
}

void Cloud::Bind() const {
    glBindVertexArray(m_vao);
}

void Cloud::UnBind() const {
    glBindVertexArray(0);
}

void Cloud::DrawOnly() const {
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
}

void Cloud::Draw() const {
    Bind();
    DrawOnly();
    UnBind();
}

void Cloud::Destroy() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    Clear();
}

void Cloud::GenerateVertices() {
    glm::vec3 halfSize = m_Size / 2.0f;

    m_vertices = {
        { {  halfSize.x,  halfSize.y, -halfSize.z }, { 1.0, 1.0, 0.0 } }, // 右上後
        { {  halfSize.x, -halfSize.y, -halfSize.z }, { 1.0, 0.0, 0.0 } }, // 右下後
        { { -halfSize.x, -halfSize.y, -halfSize.z }, { 0.0, 0.0, 0.0 } }, // 左下後
        { { -halfSize.x,  halfSize.y, -halfSize.z }, { 0.0, 1.0, 0.0 } }, // 左上後
        { {  halfSize.x,  halfSize.y,  halfSize.z }, { 1.0, 1.0, 1.0 } }, // 右上前
        { {  halfSize.x, -halfSize.y,  halfSize.z }, { 1.0, 0.0, 1.0 } }, // 右下前
        { { -halfSize.x, -halfSize.y,  halfSize.z }, { 0.0, 0.0, 1.0 } }, // 左下前
        { { -halfSize.x,  halfSize.y,  halfSize.z }, { 0.0, 1.0, 1.0 } }, // 左上前
    };

    m_indices = {
        0, 1, 2,
        0, 2, 3,
        3, 2, 6,
        3, 6, 7,
        7, 6, 5,
        7, 5, 4,
        4, 5, 1,
        4, 1, 0,
        4, 0, 3,
        4, 3, 7,
        6, 2, 1,
        6, 1, 5
    };

    glm::mat4 model_matrix = glm::mat4(1.0f);
    m_WorldMatrix = glm::translate(model_matrix, m_Position);
}

void Cloud::BufferInitialize() {
    // 建立 VAO VBO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(Attributes::Position);
    glEnableVertexAttribArray(Attributes::Texture);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(VolumeVertex), m_vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(Attributes::Position, 3, GL_FLOAT, GL_FALSE, sizeof(VolumeVertex), reinterpret_cast<const void*>(offsetof(VolumeVertex, position)));
    glVertexAttribPointer(Attributes::Texture, 3, GL_FLOAT, GL_FALSE, sizeof(VolumeVertex), reinterpret_cast<const void*>(offsetof(VolumeVertex, texture_coordinate)));

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

size_t Cloud::GetTexelIndex(size_t x, size_t y, size_t z) {
    return static_cast<size_t>(z * (size_t)m_TextureSize.x * (size_t)m_TextureSize.y + y * (size_t)m_TextureSize.x + x);
}

void Cloud::CreatePerlinNoise() {
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{ seed };

    m_TextureSize = { 128, 128, 128 };
    auto pixelCount = static_cast<size_t>(m_TextureSize.x * m_TextureSize.y * m_TextureSize.z);
    m_ImageData.clear();
    m_ImageData.reserve(pixelCount);

    float frequency = 8.0f;
    const double fx = (frequency / m_TextureSize.x);
    const double fy = (frequency / m_TextureSize.y);
    const double fz = (frequency / m_TextureSize.z);

    for (size_t z = 0; z < (size_t)m_TextureSize.z; ++z) {
        for (size_t y = 0; y < (size_t)m_TextureSize.y; ++y) {
            for (size_t x = 0; x < (size_t)m_TextureSize.x; ++x) {
                const double noise = perlin.octave3D_01((float)x * fx, (float)y * fy, (float)z * fz, 8);
                auto color = static_cast<unsigned int>((noise + 1.0f) * 0.5f * 255);
                m_ImageData[GetTexelIndex(x, y, z)] = color;
            }
        }
    }

    stbi_write_png("assets/textures/perlin_noise_texture.png", m_TextureSize.x, m_TextureSize.y, 1, m_ImageData.data(), m_TextureSize.x);
}

void Cloud::GenerateTextureData() {
    m_PerlinNoiseTexture = std::make_shared<Texture3D>();
    m_PerlinNoiseTexture->Generate(GL_R8, GL_R,
                                   (int)m_TextureSize.x, (int)m_TextureSize.y, (int)m_TextureSize.z,
                                    reinterpret_cast<const void*>(m_ImageData.data()));
}

void Cloud::Clear() {
    m_vertices.clear();
    m_indices.clear();
}
