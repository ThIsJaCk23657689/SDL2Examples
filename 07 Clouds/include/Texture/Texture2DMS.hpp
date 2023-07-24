#ifndef TEXTURE2DMS_HPP
#define TEXTURE2DMS_HPP

#include <glad/glad.h>

struct Texture2DMS {
    Texture2DMS();

    void Bind(GLint texture_idx = GL_TEXTURE0) const;
    void UnBind() const;
    void Destroy() const;
    void Generate(GLint internal_format, GLenum format, int image_width,int image_height, unsigned char* image);

    void SetWrapParameters(GLint wrap_s, GLint wrap_t) const;
    void SetFilterParameters(GLint min_filter, GLint mag_filter) const;

    unsigned int id;
};

#endif
