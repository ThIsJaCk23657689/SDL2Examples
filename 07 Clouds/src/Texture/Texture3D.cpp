#include "Texture/Texture3D.hpp"

Texture3D::Texture3D() : m_id(0) {
    glGenTextures(1, &m_id);
}

void Texture3D::Active(GLint texture_idx) const {
    glActiveTexture(texture_idx);
}

void Texture3D::Bind() const {
    glBindTexture(GL_TEXTURE_3D, m_id);
}

void Texture3D::UnBind() const {
    glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::Destroy() const {
    glDeleteTextures(1, &m_id);
}

void Texture3D::SetWrapParameters(GLint wrap_s, GLint wrap_t, GLint wrap_r) const {
    Bind();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap_r);
    UnBind();
}

void Texture3D::SetFilterParameters(GLint min_filter, GLint mag_filter) const {
    Bind();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, mag_filter);
    UnBind();
}

void Texture3D::Generate(GLint internal_format, GLenum format, int width, int height, int depth, const void* data) {
    // Notice that the data type of the image data, we set GL_FLOAT here for the volume rendering.
    Bind();
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage3D(GL_TEXTURE_3D, 0, internal_format, width, height, depth, 0, format, GL_UNSIGNED_INT, data);

    SetWrapParameters(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    SetFilterParameters(GL_LINEAR, GL_LINEAR);
    UnBind();
}
