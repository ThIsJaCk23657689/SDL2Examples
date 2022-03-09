#ifndef WORLD_HPP
#define WORLD_HPP

#include <glm/glm.hpp>

#include <array>
#include <memory>
#include <random>

#include "Entity.hpp"
#include "Geometry/3D/Cube.hpp"
#include "Geometry/3D/Sphere.hpp"
#include "Geometry/2D/Screen.hpp"

#include "Material/Material.hpp"

#include "Camera.hpp"
#include "Light/Light.hpp"

struct World {
    // Geometry Shapes
    std::unique_ptr<Sphere> my_sphere = nullptr;
    std::unique_ptr<Cube> my_cube = nullptr;
    std::unique_ptr<Cube> view_volume = nullptr;
    std::unique_ptr<Screen> my_screen = nullptr;

    // Material
    std::unique_ptr<Material> sun_material = nullptr;
    std::unique_ptr<Material> earth_material = nullptr;
    std::unique_ptr<Material> moon_material = nullptr;
    std::unique_ptr<Material> rick_roll_material = nullptr;
    std::unique_ptr<Material> green_material = nullptr;
    std::unique_ptr<Material> gray_material = nullptr;

//    // Model
//    std::vector<std::unique_ptr<Model>> light_balls_model;
//    std::vector<std::unique_ptr<Model>> planets_model;
//    std::vector<std::unique_ptr<Model>> rick_roll_cube_model;
//    std::vector<std::unique_ptr<Model>> big_ground_cube_model;
//    std::vector<std::unique_ptr<Model>> camera_model;
//    std::vector<std::unique_ptr<Model>> view_volume_model;

    // Entity (For movement)
    std::vector<Entity> suns;
    std::vector<Entity> earths;
    std::vector<Entity> moons;
    std::vector<Entity> rick_rolls;
    std::vector<Entity> grounds;
    std::vector<Entity> cameras;

    // Camera
    float ortho_distance = 50.0f;
    std::unique_ptr<Camera> my_camera = nullptr;
    std::unique_ptr<Camera> ortho_x_camera = nullptr;
    std::unique_ptr<Camera> ortho_y_camera = nullptr;
    std::unique_ptr<Camera> ortho_z_camera = nullptr;

    // Lighting
    std::unique_ptr<Light> my_directional_light = nullptr;
    std::array<std::unique_ptr<Light>, 6> my_point_lights = { nullptr };
    std::unique_ptr<Light> my_spotlight = nullptr;

    float speed = 50.0f;

    bool draw_axes = false;
    bool culling = true;
    bool wire_mode = false;
    float hdr_exposure = 1.0f;

    void Create();
    void Destroy();

    // TODO:: Not support to be here
    void UpdateViewVolumeVertices();
};

#endif