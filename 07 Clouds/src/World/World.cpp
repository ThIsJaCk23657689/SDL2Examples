#include "World/World.hpp"

#include "Texture/TextureManager.hpp"

std::mt19937_64 rand_generator;
std::uniform_real_distribution<float> random_num(0, 1);

void World::Create() {
    // Geometry Initialize
    my_cube = std::make_unique<Cube>();
    my_sphere = std::make_unique<Sphere>();
    view_volume = std::make_unique<Cube>();
    my_floor = std::make_unique<Floor>(10.0f);
    billboard = std::make_unique<Billboard>();
    my_screen = std::make_unique<Screen>();

    // Load Images and Create Textures
    TextureManager::Initialize();

    // Cloud
    my_cloud = std::make_unique<Cloud>(glm::vec3(0.0f, 10.0f, -40.0f), glm::vec3(20.0f, 20.0f, 20.0f),  glm::vec3(1.0f, 1.0f, 1.0f));

    // Entity Initialize
    sun = Entity(glm::vec3(16.0f, 35.0f, -33.0f), glm::vec3(0.0f), glm::vec3(3.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    sun.material.emission_texture = true;
    //earth = Entity(glm::vec3(20.0f, 10.0f, 0.0f), glm::vec3(0.0f, 23.5f, 0.0f), glm::vec3(1.0f), &TextureManager::GetTexture2D("Earth"));
    //earth.material.shininess = 256.0f;
    //moon = Entity(glm::vec3(25.0f, 10.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.6f), &TextureManager::GetTexture2D("Moon"));
    //moon.material.shininess = 256.0f;
    ground = Entity(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(100.0f), &TextureManager::GetTexture2D("Wooden Floor"));
    camera = Entity(glm::vec3(0.0f, 10.0f, 80.0f), glm::vec3(0.0f), glm::vec3(8.0f, 5.0f, 10.0f), glm::vec3(0.2f));
    camera.movable = true;
    //brick = Entity(glm::vec3(0.0f, 5.0f, -30.0f), glm::vec3(0.0f), glm::vec3(10.0f), &TextureManager::GetTexture2D("Brick Wall"));
    //brick.material.shininess = 512.0f;
//    for (int i = 0; i < 50; i++) {
//        float x = random_num(rand_generator) * 100.0f - 50.0f;
//        float y = random_num(rand_generator) * 50.0f;
//        float z = random_num(rand_generator) * 100.0f - 50.0f;
//        float pitch = random_num(rand_generator) * 180.0f;
//        float yaw = random_num(rand_generator) * 180.0f;
//        float scale = random_num(rand_generator) + 1.0f;
//
//        auto temp = Entity(glm::vec3(x, y, z),glm::vec3(pitch, yaw, 0), glm::vec3(scale), &TextureManager::GetTexture2D("RickRoll"));
//        rick_rolls.emplace_back(temp);
//    }
//    for (int i = 0; i < 40; i++) {
//        float x = random_num(rand_generator) * 100.0f - 50.0f;
//        float z = random_num(rand_generator) * 100.0f - 50.0f;
//        auto temp = Entity(glm::vec3(x, 0.0f, z), {0, 0, 0}, glm::vec3(1.5f), &TextureManager::GetTexture2D("Awesome Face"));
//        awesome_faces.emplace_back(temp);
//    }

    // Volumetric Cloud
    // cloud = Entity(glm::vec3(0.0f, 5.0f, -5.0f), glm::vec3(0.0f), glm::vec3(10.0f), glm::vec3(0.78572f, 0.2578f, 0.863f));
    // cloud.material.shininess = 512.0f;

    // Player Initialize
    me = std::make_unique<Player>(&camera);

    // Camera Initialize
    my_camera = std::make_unique<Camera>(glm::vec3(0.0, 0.0, 50.0), my_cloud->GetPosition(), true);
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
    my_directional_light = std::make_unique<Light>(glm::vec4(0.0f, -1.0f, -3.0f, 0.0f), true);
    my_spotlight = std::make_unique<Light>(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, -0.5f, -1.0f), true);
    my_spotlight->HookCamera(my_camera.get());

    my_directional_light->UpdateColor(glm::vec3(1.5f, 1.5f, 1.5f));
    my_point_lights = {
        std::make_unique<Light>(glm::vec3(33.75f, 8.75f, -37.654f), false),
        std::make_unique<Light>(glm::vec3(-23.75f, 7.5f, 37.654f), false),
        std::make_unique<Light>(glm::vec3(-17.5f, 8.75f, -32.716f), false),
        std::make_unique<Light>(glm::vec3(32.5f, 7.5f, 23.309f), false),
        std::make_unique<Light>(glm::vec3(-42.5f, 12.5f, -5.556f), false),
        std::make_unique<Light>(glm::vec3(sun.position), true),
    };
    my_point_lights[0]->UpdateColor(glm::vec3(10.0f, 10.0f, 0.0f));
    my_point_lights[1]->UpdateColor(glm::vec3(10.0f, 0.0f, 0.0f));
    my_point_lights[2]->UpdateColor(glm::vec3(0.0f, 0.0f, 15.0f));
    my_point_lights[3]->UpdateColor(glm::vec3(0.0f, 5.0f, 0.0f));
    my_point_lights[4]->UpdateColor(glm::vec3(20.0f, 0.0f, 20.0f));
    my_point_lights[5]->UpdateColor(glm::vec3(200.0f, 200.0f, 200.0f));

    // Fog init
    my_fog = std::make_unique<Fog>(glm::vec3(0.0f), 0.008f, 1.5f, 50.0f, 120.0f);
    my_fog->mode = FogMode::EXP;
}

void World::Destroy() {
    TextureManager::Destroy();
}