#include "Texture/Texture2DMS.hpp"

Texture2DMS::Texture2DMS() : id(0) {
    glGenTextures(1, &id);
}

void Texture2DMS::Bind(GLint texture_idx) const {
    glActiveTexture(texture_idx);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
}

void Texture2DMS::UnBind() const {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void Texture2DMS::Destroy() const {
    glDeleteTextures(1, &id);
}

void Texture2DMS::SetWrapParameters(GLint wrap_s, GLint wrap_t) const {
    Bind();
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, wrap_t);
    UnBind();
}

void Texture2DMS::SetFilterParameters(GLint min_filter, GLint mag_filter) const {
    Bind();
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, mag_filter);
    UnBind();
}

void Texture2DMS::Generate(GLint internal_format, GLenum format, int image_width,int image_height, unsigned char* image) {
    Bind();
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, internal_format, image_width, image_height, GL_TRUE);

    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, (format == GL_RGBA) ? GL_CLAMP_TO_EDGE : GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, (format == GL_RGBA) ? GL_CLAMP_TO_EDGE : GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    UnBind();
}