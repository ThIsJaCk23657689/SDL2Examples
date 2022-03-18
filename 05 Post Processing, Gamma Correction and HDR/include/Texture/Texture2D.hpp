#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <glad/glad.h>
#include <stb_image.h>
#include <string>

struct Texture2D {
    Texture2D();
    Texture2D(int screen_width, int screen_height);

    void Bind(GLint texture_idx = GL_TEXTURE0) const;
    void UnBind() const;
    void Destroy() const;
    void Generate(GLint internal_format, GLenum format, int image_width,int image_height, unsigned char* image, bool enable_mipmap = true);
    void SetWrapParameters(GLint wrap_s, GLint wrap_t) const;
    void SetFilterParameters(GLint min_filter, GLint mag_filter) const;

    unsigned int id;
};

#endif