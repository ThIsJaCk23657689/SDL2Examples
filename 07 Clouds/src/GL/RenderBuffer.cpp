#include "GL/RenderBuffer.hpp"

RenderBuffer::RenderBuffer(unsigned int screen_width, unsigned int screen_height, bool is_multisampled)
    : m_IsMultiSampled(is_multisampled) {
    glGenRenderbuffers(1, &ID);
    Bind();

    if (m_IsMultiSampled) {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, screen_width, screen_height);
    } else {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screen_width, screen_height);
    }

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
void RenderBuffer::Resize(unsigned int screen_width, unsigned int screen_height) {
    Bind();

    if (m_IsMultiSampled) {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, screen_width, screen_height);
    } else {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screen_width, screen_height);
    }

    UnBind();
}