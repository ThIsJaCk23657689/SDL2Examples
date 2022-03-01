#pragma once

#include <glm/glm.hpp>

#include <array>
#include <memory>

#include "Entity.hpp"
#include "Geometry/3D/Cube.hpp"
#include "Geometry/3D/Sphere.hpp"
#include "Geometry/2D/Screen.hpp"

#include "Camera.hpp"
#include "Light/Light.hpp"

struct World {
    // Geometry Shapes
    std::unique_ptr<Sphere> my_sphere = nullptr;
    std::unique_ptr<Cube> my_cube = nullptr;
    std::unique_ptr<Cube> view_volume = nullptr;
    std::unique_ptr<Screen> my_screen = nullptr;

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

    // Material
    float shininess = 4.0f;

    // Lighting
    std::unique_ptr<Light> my_directional_light = nullptr;
    std::array<std::unique_ptr<Light>, 6> my_point_lights = { nullptr };
    std::unique_ptr<Light> my_spotlight = nullptr;

    float speed = 50.0f;

    bool draw_axes = false;
    bool culling = false;
    bool wire_mode = false;
    float hdr_exposure = 1.0f;

    void create();
    // void destroy();

    void UpdateViewVolumeVertices();
};