#ifndef SCREENSHADER_HPP
#define SCREENSHADER_HPP

#include "Shader.hpp"

struct ScreenShader : public Shader {
    ScreenShader();

private:
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;

};

#endif
