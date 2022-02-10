#include "Texture2D.hpp"

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

void Texture2D::Bind() {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture2D::UnBind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}