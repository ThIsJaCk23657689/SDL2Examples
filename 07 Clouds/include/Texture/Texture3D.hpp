#ifndef TEXTURE3D_HPP
#define TEXTURE3D_HPP

#include <glad/glad.h>
#include <string>

struct Texture3D {
    Texture3D();

    void Active(GLint texture_idx = GL_TEXTURE0) const;
    void Bind() const;
    void UnBind() const;
    void Destroy() const;
    void Generate(GLint internal_format, GLenum format, int width, int height, int depth, const void* data);

    void SetWrapParameters(GLint wrap_s, GLint wrap_t, GLint wrap_r) const;
    void SetFilterParameters(GLint min_filter, GLint mag_filter) const;

    unsigned int m_id;
};

#endif
