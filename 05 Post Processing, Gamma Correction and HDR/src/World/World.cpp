#include "World/World.hpp"

#include "Texture/TextureManager.hpp"

std::mt19937_64 rand_generator;
std::uniform_real_distribution<float> random_num(0, 1);

void World::Create() {
    // Geometry Initialize
    my_cube = std::make_unique<Cube>();
    my_sphere = std::make_unique<Sphere>();
    view_volume = std::make_unique<Cube>();
    billboard = std::make_unique<Billboard>();
    my_screen = std::make_unique<Screen>();

    // Load Images and Create Textures
    TextureManager::Initialize();

    // Entity Initialize
    sun = Entity(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f), glm::vec3(3.0f), &TextureManager::GetTexture2D("Sun"));
    sun.material.emission_texture = true;
    earth = Entity(glm::vec3(20.0f, 10.0f, 0.0f), glm::vec3(0.0f, 23.5f, 0.0f), glm::vec3(1.0f), &TextureManager::GetTexture2D("Earth"));
    moon = Entity(glm::vec3(25.0f, 10.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.6f), &TextureManager::GetTexture2D("Moon"));
    ground = Entity(glm::vec3(0.0f, -50.0f, 0.0f), glm::vec3(0.0f), glm::vec3(100.0f), glm::vec3(42 / 255.0f, 219 / 255.0f, 89 / 255.0f));
    camera = Entity(glm::vec3(0.0f, 10.0f, 80.0f), glm::vec3(0.0f), glm::vec3(8.0f, 5.0f, 10.0f), glm::vec3(0.2f));
    camera.movable = true;
    for (int i = 0; i < 50; i++) {
        float x = random_num(rand_generator) * 100.0f - 50.0f;
        float y = random_num(rand_generator) * 50.0f;
        float z = random_num(rand_generator) * 100.0f - 50.0f;
        float pitch = random_num(rand_generator) * 180.0f;
        float yaw = random_num(rand_generator) * 180.0f;
        float scale = random_num(rand_generator) + 1.0f;

        auto temp = Entity(glm::vec3(x, y, z),glm::vec3(pitch, yaw, 0), glm::vec3(scale), &TextureManager::GetTexture2D("RickRoll"));
        rick_rolls.emplace_back(temp);
    }
    for (int i = 0; i < 40; i++) {
        float x = random_num(rand_generator) * 100.0f - 50.0f;
        float z = random_num(rand_generator) * 100.0f - 50.0f;
        auto temp = Entity(glm::vec3(x, 0.0f, z), {0, 0, 0}, glm::vec3(1.5f), &TextureManager::GetTexture2D("Awesome Face"));
        awesome_faces.emplace_back(temp);
    }

    // Player Initialize
    me = std::make_unique<Player>(&camera);

    // Camera Initialize
    my_camera = std::make_unique<Camera>(true);
    my_camera->HookEntity(camera);
    ortho_x_camera = std::make_unique<Camera>(my_camera->position + glm::vec3(1.0f, 0.0f, 0.0f) * ortho_distance,
                                              my_camera->position);
    ortho_x_camera->yaw = -90.0f;
    ortho_y_camera = std::make_unique<Camera>(my_camera->position + glm::vec3(0.0f, 1.0f, 0.0f) * ortho_distance,
                                              my_camera->position);
    ortho_y_camera->pitch = -90.0f;
    ortho_y_camera->world_up = glm::vec3(0.0f, 0.0f, -1.0f);
    ortho_z_camera = std::make_unique<Camera>(my_camera->position + glm::vec3(0.0f, 0.0f, 1.0f) * ortho_distance,
                                              my_camera->position);

    // Light Initialize
    my_directional_light = std::make_unique<Light>(glm::vec4(0.0f, -1.0f, -3.0f, 0.0f), false);
    my_spotlight = std::make_unique<Light>(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, -0.5f, -1.0f), true);
    my_spotlight->HookCamera(my_camera.get());

    my_point_lights = {
        std::make_unique<Light>(glm::vec3(33.75f, 8.75f, -37.654f), true),
        std::make_unique<Light>(glm::vec3(-23.75f, 7.5f, 37.654f), true),
        std::make_unique<Light>(glm::vec3(-17.5f, 8.75f, -32.716f), true),
        std::make_unique<Light>(glm::vec3(32.5f, 7.5f, 23.309f), true),
        std::make_unique<Light>(glm::vec3(-42.5f, 12.5f, -5.556f), true),
        std::make_unique<Light>(glm::vec3(sun.position), true),
    };
    my_point_lights[0]->UpdateColor(glm::vec3(1.0f, 0.082f, 0.082f));
    my_point_lights[1]->UpdateColor(glm::vec3(0.082f, 0.082f, 1.0f));
    my_point_lights[2]->UpdateColor(glm::vec3(0.082f, 1.0f, 0.082f));
    my_point_lights[3]->UpdateColor(glm::vec3(0.82f, 0.0f, 1.0f));
    my_point_lights[4]->UpdateColor(glm::vec3(1.0f, 0.82f, 0.0f));
    my_point_lights[5]->UpdateColor(glm::vec3(165 / 255.0f, 177 / 255.0f, 1.0f));

    // Fog init
    my_fog = std::make_unique<Fog>(glm::vec3(0.1f, 0.1f, 0.1f), 0.012f, 50.0f, 120.0f);
    my_fog->mode = FogMode::LINEAR;
}

void World::Destroy() {
    TextureManager::Destroy();
}