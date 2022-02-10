#pragma once

#include <glad//glad.h>

struct RenderBuffer {
    unsigned int ID;
    RenderBuffer(unsigned int screen_width, unsigned int screen_height);
    ~RenderBuffer();
    void Bind();
    void UnBind();
};