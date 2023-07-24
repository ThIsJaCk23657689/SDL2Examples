#include "Shader/ScreenShader.hpp"

const std::string ScreenShader::VERTEX_FILE = "assets/shaders/screen.vert";
const std::string ScreenShader::FRAGMENT_FILE = "assets/shaders/screen.frag";

ScreenShader::ScreenShader() : Shader(VERTEX_FILE, FRAGMENT_FILE) {}