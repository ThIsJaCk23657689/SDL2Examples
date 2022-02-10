#include "RenderBuffer.hpp"

RenderBuffer::RenderBuffer(unsigned int screen_width, unsigned int screen_height) {
    glGenRenderbuffers(1, &ID);
    Bind();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screen_width, screen_height);
    UnBind();
}

RenderBuffer::~RenderBuffer() {
    glDeleteRenderbuffers(1, &ID);
}

void RenderBuffer::Bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, ID);
}

void RenderBuffer::UnBind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}