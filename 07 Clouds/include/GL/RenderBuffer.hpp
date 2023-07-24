#ifndef RENDERBUFFER_HPP
#define RENDERBUFFER_HPP

#include <glad//glad.h>

struct RenderBuffer {
    unsigned int ID;
    bool m_IsMultiSampled = false;

    RenderBuffer(unsigned int screen_width, unsigned int screen_height, bool is_multisampled = false);
    ~RenderBuffer();

    void Bind();
    void UnBind();
    void Resize(unsigned int screen_width, unsigned int screen_height);
};
#endif