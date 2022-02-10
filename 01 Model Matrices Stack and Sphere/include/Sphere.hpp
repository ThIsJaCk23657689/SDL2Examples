#pragma once

#include "Geometry.hpp"

#include <glad/glad.h>
#include <vector>

struct Sphere : public Geometry {

    Sphere();
    Sphere(float radius, unsigned int longitude, unsigned int latitude);
    void SetRadius(float radius);
    void SetLongitude(unsigned int longitude);
    void SetLatitude(unsigned int latitude);

protected:
    void GenerateVertices() override;

private:
    float m_radius;
    unsigned int m_longitude;
    unsigned int m_latitude;

};