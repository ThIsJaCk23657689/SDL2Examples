#include "Renderer/ViewVolumeRenderer.hpp"

ViewVolumeRenderer::ViewVolumeRenderer(AlphaShader* shader) : m_shader(shader) {
    
}

void ViewVolumeRenderer::Prepare(const std::unique_ptr<Camera>& camera) {
    m_shader->Start();
    m_shader->SetViewAndProj(camera);
}
void ViewVolumeRenderer::Render(const Geometry* geometry) {
    // Prepare Geometry (Bind VAO)
    geometry->Bind();

    // Prepare Instance
    glm::mat4 model_matrix = glm::mat4(1.0f);
    m_shader->SetMat4("model", model_matrix);

    // 繪製邊框
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_shader->SetVec4("objectColor", glm::vec4(0.1f, 0.1f, 0.1f, 0.9f));
    geometry->DrawOnly();

    // 繪製主體
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_shader->SetVec4("objectColor", glm::vec4(0.889f, 0.889f, 0.889f, 0.3f));
    geometry->DrawOnly();

    // Unbind VAO and Texture
    geometry->UnBind();
}