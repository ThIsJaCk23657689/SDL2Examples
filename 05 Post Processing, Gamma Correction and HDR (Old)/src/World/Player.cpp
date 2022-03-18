#include "World/Player.hpp"

#include "SDL.h"

Player::Player(Entity* ent) : mouse_control(false), mouse_sensitivity(0.1f), entity(ent) {}

void Player::Update(float dt) {
    entity->Update(dt);
}

// 掌管玩家事件處理，例如滑鼠與鍵盤事件
void Player::HandleEvents() {
    ProcessKeyboard();
    ProcessMouseMovement();
}

void Player::ToggleMouseControl() {
    mouse_control = !mouse_control;
}

// 玩家鍵盤事件處理
void Player::ProcessKeyboard() {
    // SDL2 鍵盤控制移動建議使用這個 SDL_GetKeyboardState()，才不會覺得卡卡頓頓的
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_LCTRL]) {
        move_speed = 450.0f;
    } else {
        move_speed = 150.0f;
    }

    if (state[SDL_SCANCODE_W]) {
        entity->acceleration += entity->front * move_speed;
    }
    if (state[SDL_SCANCODE_S]) {
        entity->acceleration -= entity->front * move_speed;
    }
    if (state[SDL_SCANCODE_D]) {
        entity->acceleration += entity->right * move_speed;
    }
    if (state[SDL_SCANCODE_A]) {
        entity->acceleration -= entity->right * move_speed;
    }
    if (state[SDL_SCANCODE_SPACE]) {
        entity->acceleration.y += move_speed * 2.0f;
    }
    if (state[SDL_SCANCODE_LSHIFT]) {
        entity->acceleration.y -= move_speed * 2.0f;
    }
}

// 玩家滑鼠事件處理
void Player::ProcessMouseMovement(bool constrain) {
    // 當 SDL_SetRelativeMouseMode() 為 TRUE 時，鼠標會不見
    // 並且使用的是【相對位置】，一般來說就是與上一幀滑鼠位置的相對位置
    // 就不用還要自己去計算，會方便許多！
    if (!mouse_control) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        return;
    }

    int xoffset, yoffset;
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_GetRelativeMouseState(&xoffset, &yoffset);

    entity->rotate.y += xoffset * mouse_sensitivity;
    entity->rotate.x += -yoffset * mouse_sensitivity;

    if (entity->rotate.y >= 360.0f) {
        entity->rotate.y = 0.0f;
    }
    if (entity->rotate.y <= -360.0f) {
        entity->rotate.y = 0.0f;
    }

    // 限制 Pitch 不能為 ±90°，不然會產生萬向鎖問題（Yaw 跟 Pitch 合併在一起，此時 Yaw 跟 Roll 會是一樣的作用）。
    if (constrain) {
        if (entity->rotate.x > 89.0f) {
            entity->rotate.x = 89.0f;
        }
        if (entity->rotate.x < -89.0f) {
            entity->rotate.x = -89.0f;
        }
    }
}