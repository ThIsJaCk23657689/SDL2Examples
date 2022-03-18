#include "Geometry/3D/Sphere.hpp"

Sphere::Sphere() : m_radius(1.0f), m_longitude(30), m_latitude(30) {
    Initialize();
}

Sphere::Sphere(float radius, unsigned int longitude, unsigned int latitude) :
    m_radius(radius), m_longitude(longitude), m_latitude(latitude) {
    Initialize();
}

void Sphere::SetRadius(float radius) {
    m_radius = radius;
    Clear();
    GenerateVertices();
}

void Sphere::SetLongitude(unsigned int longitude) {
    m_longitude = longitude;
    Clear();
    GenerateVertices();
}

void Sphere::SetLatitude(unsigned int latitude) {
    m_latitude = latitude;
    Clear();
    GenerateVertices();
}

void Sphere::GenerateVertices() {
    // 定義頂點（逆時針）與索引
    for (int i = 0; i <= m_latitude; ++i) {
        float theta = i * PI / m_latitude;
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (int j = 0; j <= m_longitude; ++j) {
            float phi = j * 2.0f * PI / m_longitude;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            // vertices order must follow counter-clockwise rules.
            float x = sinPhi * sinTheta;
            float y = cosTheta;
            float z = cosPhi * sinTheta;

            // texture coordinate (u, v) range between [0, 1]
            float u = j / static_cast<float>(m_longitude);
            float v = 1.0f - (i / static_cast<float>(m_latitude));

            // 新增圓球座標位置到頂點中, 其法向量就等同於圓心（原點）到半徑為１的球表面上（單位向量）。
            vertices.push_back({ {m_radius * x, m_radius * y, m_radius * z}, { x, y, z }, { u, v } });
        }
    }

    for (int i = 0; i < m_latitude; ++i) {
        for (int j = 0; j < m_longitude; ++j) {
            int first = i * (m_longitude + 1) + j;
            int second = first + m_longitude + 1;

            // counter-clockwise
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
        }
    }
}