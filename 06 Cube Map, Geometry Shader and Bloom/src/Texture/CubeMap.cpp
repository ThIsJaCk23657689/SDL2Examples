#include "Texture/CubeMap.hpp"

CubeMap::CubeMap() : id(0) {
    glGenTextures(1, &id);
}

void CubeMap::Bind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void CubeMap::UnBind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::Active(GLint texture_idx) const {
    glActiveTexture(texture_idx);
}

void CubeMap::Destroy() const {
    glDeleteTextures(1, &id);
}

void CubeMap::BindImage(GLenum cube_map_index, GLint internal_format, GLenum format, int image_width, int image_height, unsigned char* image) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + cube_map_index, 0, internal_format, image_width, image_height, 0, format, GL_UNSIGNED_BYTE, image);
}

void CubeMap::SetMipmap(bool is_enable) const {
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void CubeMap::SetWrapParameters(GLint wrap_s, GLint wrap_t, GLint wrap_r) const {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_r);
}

void CubeMap::SetFilterParameters(GLint min_filter, GLint mag_filter) const {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mag_filter);
}