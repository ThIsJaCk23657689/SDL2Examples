#ifndef ALPHARENDERER_HPP
#define ALPHARENDERER_HPP

#include "Renderer/Renderer.hpp"
#include "Shader/AlphaShader.hpp"

struct AlphaRenderer : public Renderer {
    AlphaRenderer(AlphaShader* shader);
    void Prepare(const std::unique_ptr<Camera>& camera) override;
    void Render(const std::vector<Entity>& entities, const std::unique_ptr<Material>& material, const Geometry* geometry) override;

private:
    AlphaShader* m_shader;
};

#endif
