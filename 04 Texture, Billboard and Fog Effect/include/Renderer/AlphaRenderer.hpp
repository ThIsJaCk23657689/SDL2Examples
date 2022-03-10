#ifndef ALPHARENDERER_HPP
#define ALPHARENDERER_HPP

#include "Renderer/Renderer.hpp"
#include "Shader/AlphaShader.hpp"

struct AlphaRenderer : public Renderer {
    AlphaRenderer(AlphaShader* shader);
    void Prepare(const std::unique_ptr<Camera>& camera) override;
    void Render(const Entity& entity, const Geometry* geometry) override;
    void Render(const std::vector<Entity>& entities, const Geometry* geometry) override;

private:
    AlphaShader* m_shader;
};

#endif
