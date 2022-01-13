#include "World.hpp"

// Global world
World my_world;

void World::create() {
    my_cube = std::make_unique<Cube>();
    my_sphere = std::make_unique<Sphere>();

    sun = std::make_unique<Entity>();
    sun->scale = glm::vec3(3.0f, 3.0f, 3.0f);

    earth = std::make_unique<Entity>();
    earth->position = glm::vec3(30.0f, 0.0f, 0.0f);

    moon = std::make_unique<Entity>();
    moon->position = glm::vec3(5.0f, 0.0f, 0.0f);
    moon->scale = glm::vec3(0.5f, 0.5f, 0.5f);
}