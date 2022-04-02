#ifndef NORMALMAPSHADER_HPP
#define NORMALMAPSHADER_HPP

#include <string>
#include <memory>

#include "Shader.hpp"
#include "Light/Light.hpp"
#include "World/Fog.hpp"

struct NormalMapShader : public Shader {
    NormalMapShader();

    void SetLight(const std::unique_ptr<Light>& light, const int& light_index = 0);

private:
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;

};


#endif