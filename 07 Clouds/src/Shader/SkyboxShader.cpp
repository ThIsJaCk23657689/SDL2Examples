#include "Shader/SkyboxShader.hpp"

const std::string SkyboxShader::VERTEX_FILE = "assets/shaders/skybox.vert";
const std::string SkyboxShader::FRAGMENT_FILE = "assets/shaders/skybox.frag";

SkyboxShader::SkyboxShader() : Shader(VERTEX_FILE, FRAGMENT_FILE) {}