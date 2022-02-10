#pragma once

#include <glm/glm.hpp>

#include <array>
#include <memory>

#include "Camera.hpp"
#include "Cube.hpp"
#include "Entity.hpp"
#include "Sphere.hpp"

struct World {
    // Geometry Shapes
    std::unique_ptr<Sphere> my_sphere = nullptr;
    std::unique_ptr<Cube> my_cube = nullptr;
    std::unique_ptr<Cube> view_volume = nullptr;

    // Entity (For movement)
    std::unique_ptr<Entity> sun = nullptr;
    std::unique_ptr<Entity> earth = nullptr;
    std::unique_ptr<Entity> moon = nullptr;

    // Camera
    float ortho_distance = 50.0f;
    std::unique_ptr<Camera> my_camera = nullptr;
    std::unique_ptr<Camera> ortho_x_camera = nullptr;
    std::unique_ptr<Camera> ortho_y_camera = nullptr;
    std::unique_ptr<Camera> ortho_z_camera = nullptr;

    float speed = 50.0f;
    bool drawaxes = false;
    bool culling = true;

    void create();
    // void destroy();
};

extern World my_world;