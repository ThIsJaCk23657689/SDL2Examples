#ifndef ALPHARENDERER_HPP
#define ALPHARENDERER_HPP

#include "Renderer/Renderer.hpp"
#include "Shader/AlphaShader.hpp"

struct ViewVolumeRenderer : public Renderer {
    explicit ViewVolumeRenderer(AlphaShader* shader);
    void Prepare(const std::unique_ptr<Camera>& camera) override;
    void Render(const Geometry* geometry);

private:
    AlphaShader* m_shader;
};

#endif
