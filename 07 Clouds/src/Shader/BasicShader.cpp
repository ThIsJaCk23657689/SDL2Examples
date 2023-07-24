#include "Shader/BasicShader.hpp"

const std::string BasicShader::VERTEX_FILE = "assets/shaders/basic.vert";
const std::string BasicShader::FRAGMENT_FILE = "assets/shaders/basic.frag";

BasicShader::BasicShader() :
    Shader(VERTEX_FILE, FRAGMENT_FILE) {
}

