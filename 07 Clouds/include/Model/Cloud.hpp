#ifndef CLOUD_HPP
#define CLOUD_HPP

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include <memory>

class Texture3D;
struct VolumeVertex;

class Cloud {
public:
    Cloud(const glm::vec3& pos, const glm::vec3& size, const glm::vec3& color);
    ~Cloud();

    void Initialize();
    void Bind() const;
    void UnBind() const;
    void DrawOnly() const;
    void Draw() const;
    void Destroy();

    const glm::vec3& GetPosition() const { return m_Position; }
    const glm::vec3& GetSize() const { return m_Size; }
    const glm::vec3& GetColor() const { return m_Color; }
    const glm::mat4& GetModelMatrix() const { return m_WorldMatrix; }
    size_t GetTexelIndex(size_t x, size_t y, size_t z);

    const std::shared_ptr<Texture3D>& GetPerlinNoiseTexture3D() const { return m_PerlinNoiseTexture; }

    int SampleCount = 64;
    float Absorbance = 1.0f;

private:
    void GenerateVertices();
    void BufferInitialize();
    void CreatePerlinNoise();
    void GenerateTextureData();
    void Clear();

    // Temp
    float PerlinNoise(float x, float y, float z);

    glm::vec3 m_Position;
    glm::vec3 m_Size;
    glm::vec3 m_Color;
    glm::mat4 m_WorldMatrix;

    GLuint m_vao, m_vbo, m_ebo;
    std::vector<VolumeVertex> m_vertices;
    std::vector<uint32_t> m_indices;

    // Noise
    glm::vec3 m_TextureSize;
    std::vector<unsigned int> m_ImageData;
    std::shared_ptr<Texture3D> m_PerlinNoiseTexture = nullptr;
};

#endif
