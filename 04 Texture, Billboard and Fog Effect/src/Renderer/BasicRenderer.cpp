#include "Renderer/BasicRenderer.hpp"

BasicRenderer::BasicRenderer(BasicShader* shader) : m_shader(shader) {

}

void BasicRenderer::Prepare(const std::unique_ptr<Camera>& camera) {
    m_shader->Start();
    m_shader->SetVec3("objectColor", glm::vec3(0.0f));

    // Load View, Projection Matrix
    m_shader->SetViewAndProj(camera);
}

void BasicRenderer::Render(const Entity& entity, const Geometry* geometry) {
    // Prepare Geometry (Bind VAO)
    geometry->Bind();

    // 這個 renderer 不在乎 texture，只在乎顏色，而且不會計算光照
    m_shader->SetVec3("objectColor", entity.material.color);

    // Prepare Instance
    glm::mat4 model_matrix = entity.GetModelMatrix();
    m_shader->SetMat4("model", model_matrix);

    // Draw
    geometry->DrawOnly();

    // Unbind VAO and Texture
    geometry->UnBind();
}

void BasicRenderer::Render(const std::vector<Entity>& entities, const Geometry* geometry) {
    // Prepare Geometry (Bind VAO)
    geometry->Bind();

    for (const auto& entity : entities) {
        // 這個 renderer 不在乎 texture，只在乎顏色，而且不會計算光照
        m_shader->SetVec3("objectColor", entity.material.color);

        // Prepare Instance
        glm::mat4 model_matrix = entity.GetModelMatrix();
        m_shader->SetMat4("model", model_matrix);

        // Draw
        geometry->DrawOnly();
    }

    // Unbind VAO and Texture
    geometry->UnBind();
}