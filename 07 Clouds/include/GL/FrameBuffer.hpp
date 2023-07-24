#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <glad/glad.h>

#include <vector>
#include <memory>
#include <iostream>

#include "Texture/Texture2D.hpp"
#include "Texture/Texture2DMS.hpp"
#include "RenderBuffer.hpp"

struct FrameBuffer {
    unsigned int ID;
    std::vector<unsigned int> attachments;

    FrameBuffer();
    ~FrameBuffer();
    void Bind();
    void UnBind();
    void SetDrawBufferAmount(int amount);
    void BindTexture2D(const Texture2D& texture, unsigned int attachment = 0);
    void BindTexture2DMS(const Texture2DMS& texture, unsigned int attachment = 0);
    void BindRenderBuffer(const std::unique_ptr<RenderBuffer>& rbo);
    void CheckComplete();

    static void BlitTexture(const FrameBuffer* from, const FrameBuffer* to, uint32_t width, uint32_t height);
};
#endif