#ifndef BASICRENDERER_HPP
#define BASICRENDERER_HPP

#include "Renderer/Renderer.hpp"
#include "Shader/BasicShader.hpp"

struct BasicRenderer : public Renderer {
    BasicRenderer(BasicShader* shader);
    void Prepare(const std::unique_ptr<Camera>& camera) override;
    void Render(const Entity& entity, const Geometry* geometry);
    void Render(const std::vector<Entity>& entities, const Geometry* geometry);

private:
    BasicShader* m_shader;
};

#endif