#include "Texture/Texture2D.hpp"

Texture2D::Texture2D() {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
}

Texture2D::Texture2D(unsigned int screen_width, unsigned int screen_height) {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &ID);
}

void Texture2D::Clear() const {
    glDeleteTextures(1, &ID);
}

void Texture2D::Bind() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture2D::UnBind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::SetWrapParameters(GLenum wrap_s, GLenum wrap_t) const {
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    UnBind();
}

void Texture2D::SetFilterParameters(GLenum min_filter, GLenum mag_filter) const {
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    UnBind();
}

void Texture2D::Generate(GLenum internal_format, GLenum format, const int& image_width, const int& image_height, const unsigned char* image, bool enable_mipmap) {
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, image_width, image_height, 0, format, GL_UNSIGNED_BYTE, image);

    if (enable_mipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (format == GL_RGBA) ? GL_CLAMP_TO_EDGE : GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (format == GL_RGBA) ? GL_CLAMP_TO_EDGE : GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (enable_mipmap) ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
    UnBind();
}