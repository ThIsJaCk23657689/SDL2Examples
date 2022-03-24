#ifndef WORLD_HPP
#define WORLD_HPP

#include <glm/glm.hpp>

#include <array>
#include <memory>
#include <random>

#include "Geometry/3D/Cube.hpp"
#include "Geometry/3D/Sphere.hpp"
#include "Geometry/2D/Floor.hpp"
#include "Geometry/2D/Billboard.hpp"
#include "Geometry/2D/Screen.hpp"

#include "Entity.hpp"
#include "Material/Material.hpp"
#include "Player.hpp"

#include "Camera.hpp"
#include "Light/Light.hpp"
#include "Fog.hpp"

enum PostEffect : unsigned int {
    NORMAL = 0,
    INVERSION = 1,
    GRAYSCALE = 2,
    NARCOTIC = 3,
    BLUR = 4,
    EDGE_DETECTION = 5,
};

enum HDRMode : unsigned int {
    REINHARD = 0,
    EXPOSURE = 1,
};

struct World {
    // Geometry Shapes
    std::unique_ptr<Sphere> my_sphere = nullptr;
    std::unique_ptr<Cube> my_cube = nullptr;
    std::unique_ptr<Cube> view_volume = nullptr;
    std::unique_ptr<Floor> my_floor = nullptr;
    std::unique_ptr<Billboard> billboard = nullptr;
    std::unique_ptr<Screen> my_screen = nullptr;

    // Entity (For movement)
    Entity sun;
    Entity earth;
    Entity moon;
    std::vector<Entity> rick_rolls;
    Entity ground;
    Entity camera;
    std::vector<Entity> awesome_faces;

    // Player
    std::unique_ptr<Player> me = nullptr;

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

    // Fog
    std::unique_ptr<Fog> my_fog = nullptr;
    bool fog_bind_bg_color = false;

    // Screen
    PostEffect current_screen_mode = PostEffect::NORMAL;

    // Bloom
    bool use_bloom = true;
    float bloom_intensity = 1.0f;
    float bloom_threshold = 1.0f;
    int bloom_strength = 10;

    float speed = 50.0f;

    bool draw_axes = false;
    bool culling = true;
    bool wire_mode = false;

    bool use_gamma_correction = true;
    float gamma_value = 2.2f;

    bool use_hdr = true;
    HDRMode current_hdr_mode = HDRMode::EXPOSURE;
    float hdr_exposure = 1.0f;

    void Create();
    void Destroy();
};

#endif