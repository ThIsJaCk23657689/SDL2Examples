#pragma once

#include <glm/glm.hpp>

#include <array>
#include <memory>

#include "Cube.hpp"
#include "Sphere.hpp"
#include "Entity.hpp"

struct World {
    // Geometry Shapes
    std::unique_ptr<Sphere> my_sphere = nullptr;
    std::unique_ptr<Cube> my_cube = nullptr;

    // Entity (For movement)
    std::unique_ptr<Entity> sun = nullptr;
    std::unique_ptr<Entity> earth = nullptr;
    std::unique_ptr<Entity> moon = nullptr;

    float speed = 50.0f;
    bool drawaxes = false;

    void create();
    // void destroy();
};

extern World my_world;