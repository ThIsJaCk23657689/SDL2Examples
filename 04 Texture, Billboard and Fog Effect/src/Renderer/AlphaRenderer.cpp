#include "Renderer/AlphaRenderer.hpp"

AlphaRenderer::AlphaRenderer(AlphaShader* shader) : m_shader(shader) {
    
}

void AlphaRenderer::Prepare(const std::unique_ptr<Camera>& camera) {
    
}

void AlphaRenderer::Render(const Entity& entity, const Geometry* geometry) {

}

void AlphaRenderer::Render(const std::vector<Entity>& entities, const Geometry* geometry) {
    
}