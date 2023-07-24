#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include <glad/glad.h>
#include <stb_image.h>
#include <vector>
#include <string>

struct CubeMap {
    CubeMap();

    void Bind() const;
    void UnBind() const;
    void Active(GLint texture_idx = GL_TEXTURE0) const;
    void Destroy() const;

    void BindImage(GLenum cube_map_index, GLint internal_format, GLenum format, int image_width, int image_height, unsigned char* image);

    void SetMipmap(bool is_enable) const;
    void SetWrapParameters(GLint wrap_s, GLint wrap_t, GLint wrap_r) const;
    void SetFilterParameters(GLint min_filter, GLint mag_filter) const;

    unsigned int id;
};

#endif
