#pragma once

#include <glad/glad.h>
#include <string>

struct Texture2D {
    unsigned int ID;

    Texture2D();
    Texture2D(unsigned int screen_width, unsigned int screen_height);
    ~Texture2D();

    void Clear() const;
    void Bind() const;
    void UnBind() const;
    void Generate(GLenum internal_format, GLenum format, const int& image_width, const int& image_height, const unsigned char* image, bool enable_mipmap = true);
    void SetWrapParameters(GLenum wrap_s, GLenum wrap_t) const;
    void SetFilterParameters(GLenum min_filter, GLenum mag_filter) const;

};