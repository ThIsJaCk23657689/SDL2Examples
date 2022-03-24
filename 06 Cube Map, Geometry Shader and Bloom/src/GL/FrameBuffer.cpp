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