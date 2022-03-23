#ifndef BASICSHADER_HPP
#define BASICSHADER_HPP

#include "Shader.hpp"

#include <string>

struct BasicShader : public Shader {
    BasicShader();

private:
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
};

#endif
