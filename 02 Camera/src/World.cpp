#include "World.hpp"

// Global world
World my_world;

void World::create() {
    my_cube = std::make_unique<Cube>();
    my_sphere = std::make_unique<Sphere>();
    view_volume = std::make_unique<Cube>();

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
}