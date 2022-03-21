#ifndef RENDERBUFFER_HPP
#define RENDERBUFFER_HPP

#include <glad//glad.h>

struct RenderBuffer {
    unsigned int ID;
    RenderBuffer(unsigned int screen_width, unsigned int screen_height);
    ~RenderBuffer();

    void Bind();
    void UnBind();
    void Resize(unsigned int screen_width, unsigned int screen_height);
};
#endif