#include "World.hpp"

// Global world
World my_world;

void World::create() {
    my_cube = std::make_unique<Cube>();
    my_sphere = std::make_unique<Sphere>();
    view_volume = std::make_unique<Cube>();
    my_screen = std::make_unique<Screen>();

    sun = std::make_unique<Entity>();
    sun->scale = glm::vec3(3.0f, 3.0f, 3.0f);

    earth = std::make_unique<Entity>();
    earth->position = glm::vec3(30.0f, 0.0f, 0.0f);

    moon = std::make_unique<Entity>();
    moon->position = glm::vec3(5.0f, 0.0f, 0.0f);
    moon->scale = glm::vec3(0.5f, 0.5f, 0.5f);

    my_camera = std::make_unique<Camera>(glm::vec3(0.0f, 10.0f, 80.0f), true);
    ortho_x_camera = std::make_unique<Camera>(my_camera->Position + glm::vec3(1.0f, 0.0f, 0.0f) * ortho_distance,
                                              my_camera->Position);
    ortho_x_camera->Yaw = -90.0f;
    ortho_y_camera = std::make_unique<Camera>(my_camera->Position + glm::vec3(0.0f, 1.0f, 0.0f) * ortho_distance,
                                              my_camera->Position);
    ortho_y_camera->Pitch = -90.0f;
    ortho_y_camera->WorldUp = glm::vec3(0.0f, 0.0f, -1.0f);
    ortho_z_camera = std::make_unique<Camera>(my_camera->Position + glm::vec3(0.0f, 0.0f, 1.0f) * ortho_distance,
                                              my_camera->Position);

    my_directional_light = std::make_unique<Light>(glm::vec4(0.0f, -1.0f, -3.0f, 0.0f), false);
    my_point_lights = {
        std::make_unique<Light>(glm::vec3(33.75f, 8.75f, -37.654f), true),
        std::make_unique<Light>(glm::vec3(-23.75f, 7.5f, 37.654f), true),
        std::make_unique<Light>(glm::vec3(-17.5f, 8.75f, -32.716f), true),
        std::make_unique<Light>(glm::vec3(32.5f, 7.5f, 23.309f), true),
        std::make_unique<Light>(glm::vec3(-42.5f, 12.5f, -5.556f), true),
        std::make_unique<Light>(glm::vec3(0.0f, 13.75f, 0.617f), true),
    };
    my_spotlight = std::make_unique<Light>(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, -0.5f, -1.0f), true);

    my_point_lights[0]->UpdateColor(glm::vec3(1.0f, 0.082f, 0.082f));
    my_point_lights[1]->UpdateColor(glm::vec3(0.082f, 0.082f, 1.0f));
    my_point_lights[2]->UpdateColor(glm::vec3(0.082f, 1.0f, 0.082f));
    my_point_lights[3]->UpdateColor(glm::vec3(0.82f, 0.0f, 1.0f));
    my_point_lights[4]->UpdateColor(glm::vec3(1.0f, 0.82f, 0.0f));
    my_point_lights[5]->UpdateColor(glm::vec3(200.0f, 200.0f, 200.0f));
}