#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <glad/glad.h>
#include <memory>
#include <iostream>
#include "Texture/Texture2D.hpp"
#include "RenderBuffer.hpp"

struct FrameBuffer {
    unsigned int ID;

    FrameBuffer();
    ~FrameBuffer();
    void Bind();
    void UnBind();
    void BindTexture2D(const std::unique_ptr<Texture2D>& texture);
    void BindRenderBuffer(const std::unique_ptr<RenderBuffer>& rbo);
    void CheckComplete();
};
#endif