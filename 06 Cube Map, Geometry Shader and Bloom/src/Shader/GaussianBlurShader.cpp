#include "Shader/GaussianBlurShader.hpp"

const std::string GaussianBlurShader::VERTEX_FILE = "assets/shaders/gaussianblur.vert";
const std::string GaussianBlurShader::FRAGMENT_FILE = "assets/shaders/gaussianblur.frag";

GaussianBlurShader::GaussianBlurShader() : Shader(VERTEX_FILE, FRAGMENT_FILE) {}