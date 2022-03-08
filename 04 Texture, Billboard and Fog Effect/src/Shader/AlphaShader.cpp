#include "Shader/AlphaShader.hpp"

const std::string AlphaShader::VERTEX_FILE = "assets/shaders/alpha.vert";
const std::string AlphaShader::FRAGMENT_FILE = "assets/shaders/alpha.frag";

AlphaShader::AlphaShader() :
    Shader(VERTEX_FILE, FRAGMENT_FILE) {
}
