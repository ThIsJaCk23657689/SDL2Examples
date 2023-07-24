#include "GL/FrameBuffer.hpp"

FrameBuffer::FrameBuffer() {
    attachments = { GL_COLOR_ATTACHMENT0 };
    glGenFramebuffers(1, &ID);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &ID);
}

void FrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FrameBuffer::UnBind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::SetDrawBufferAmount(int amount) {
    attachments.clear();
    for (int i = 0; i < amount; i++) {
        attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    Bind();
    glDrawBuffers(amount, attachments.data());
    UnBind();
}

void FrameBuffer::BindTexture2D(const Texture2D& texture, unsigned int attachment) {
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D, texture.id, 0);
    UnBind();
}

void FrameBuffer::BindTexture2DMS(const Texture2DMS& texture, unsigned int attachment) {
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D_MULTISAMPLE, texture.id, 0);
    UnBind();
}

void FrameBuffer::BindRenderBuffer(const std::unique_ptr<RenderBuffer>& rbo) {
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo->ID);
    UnBind();
}

void FrameBuffer::CheckComplete() {
    Bind();
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR: Framebuffer is not complete at Framebuffer ID: " + std::to_string(ID) << std::endl;
    }
    UnBind();
}




void FrameBuffer::BlitTexture(const FrameBuffer* from, const FrameBuffer* to, uint32_t width, uint32_t height) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, from->ID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, to->ID);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}