#ifndef MASTERRENDERER_HPP
#define MASTERRENDERER_HPP

#include <memory>
#include <vector>

#include "Camera.hpp"

#include "Shader/BasicShader.hpp"
#include "Shader/LightningShader.hpp"
#include "Shader/AlphaShader.hpp"

#include "Renderer/BasicRenderer.hpp"
#include "Renderer/LightningRenderer.hpp"
#include "Renderer/AlphaRenderer.hpp"

#include "World/Entity.hpp"

struct MasterRenderer {
    MasterRenderer();

    void Initialize();
    void Render(const std::unique_ptr<Camera>& camera);
    void Destroy();

private:
    // Shaders
    std::unique_ptr<BasicShader> basic_shader = nullptr;
    std::unique_ptr<LightningShader> lightning_shader = nullptr;
    std::unique_ptr<AlphaShader> alpha_shader = nullptr;

    // Renderers
    std::unique_ptr<BasicRenderer> basic_renderer = nullptr;
    std::unique_ptr<LightningRenderer> lightning_renderer = nullptr;
    std::unique_ptr<AlphaRenderer> alpha_renderer = nullptr;

};

#endif