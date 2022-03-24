#ifndef GAUSSIANBLURSHADER_HPP
#define GAUSSIANBLURSHADER_HPP

#include "Shader.hpp"

struct GaussianBlurShader : public Shader {
    GaussianBlurShader();

private:
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;

};

#endif
