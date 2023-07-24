#ifndef SKYBOXSHADER_HPP
#define SKYBOXSHADER_HPP

#include "Shader/Shader.hpp"

struct SkyboxShader : Shader {
    SkyboxShader();

private:
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
};

#endif
