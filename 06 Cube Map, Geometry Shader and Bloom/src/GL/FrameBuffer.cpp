#include "GL/FrameBuffer.hpp"

FrameBuffer::FrameBuffer() {
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

void FrameBuffer::BindTexture2D(const Texture2D& texture) {
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);
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
        std::cout << "ERROR: Framebuffer is not complete!" << std::endl;
    }
    UnBind();
}