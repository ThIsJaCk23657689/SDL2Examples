#include "Renderer/BasicRenderer.hpp"

BasicRenderer::BasicRenderer(BasicShader* shader) : m_shader(shader) {

}

void BasicRenderer::Prepare(const std::unique_ptr<Camera>& camera) {

}

void BasicRenderer::Render(const std::vector<Entity>& entities, const std::unique_ptr<Material>& material, const Geometry* geometry) {

}