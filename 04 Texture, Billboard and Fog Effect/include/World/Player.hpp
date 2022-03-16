#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <glm/glm.hpp>

#include "Entity.hpp"
#include "Texture/Texture2D.hpp"

struct Player {
    Entity* entity;

    Player(Entity* ent);

    void Update(float dt);
    void HandleEvents();
    void ToggleMouseControl();

private:
    float move_speed;

    bool mouse_control;
    float mouse_sensitivity;

    void ProcessKeyboard();
    void ProcessMouseMovement(bool constrain = true);

    void Jump();

};

#endif