#ifndef LIGHTNINGRENDERER_HPP
#define LIGHTNINGRENDERER_HPP

#include "Camera.hpp"
#include "Renderer/Renderer.hpp"
#include "Shader/LightningShader.hpp"

struct LightningRenderer : public Renderer {
    LightningRenderer(LightningShader* shader);
    virtual void Prepare(const Camera* camera) override;
    void Render(const Entity& entity, const Geometry* geometry);
    void Render(const std::vector<Entity>& entities, const Geometry* geometry);

private:
    LightningShader* m_shader;
};

#endif
