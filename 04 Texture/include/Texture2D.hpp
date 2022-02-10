#pragma once

#include <glad/glad.h>

struct Texture2D {
    unsigned int ID;

    Texture2D(unsigned int screen_width, unsigned int screen_height);
    ~Texture2D();
    void Bind();
    void UnBind();
};