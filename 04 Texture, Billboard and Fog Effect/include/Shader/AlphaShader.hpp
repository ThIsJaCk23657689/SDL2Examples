#ifndef ALPHASHADER_HPP
#define ALPHASHADER_HPP

#include "Shader.hpp"

#include <string>

struct AlphaShader : public Shader {
    AlphaShader();

private:
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
};

#endif