#include "Renderer/LightningRenderer.hpp"

LightningRenderer::LightningRenderer(LightningShader* shader) : m_shader(shader) {
    
}

void LightningRenderer::Prepare(const std::unique_ptr<Camera>& camera) {
    m_shader->Start();
    m_shader->SetBool("useLighting", true);
    m_shader->SetBool("useBlinnPhong", true);
    m_shader->SetVec3("objectColor", glm::vec3(0.0f));
    m_shader->SetBool("useTexture", false);
    m_shader->SetInt("diffuse_texture", 0);

    // Load Lights
    m_shader->SetLight(state.world->my_directional_light, 0);
    m_shader->SetLight(state.world->my_spotlight, 1);
    for (int i = 0; i < state.world->my_point_lights.size(); ++i) {
        m_shader->SetLight(state.world->my_point_lights[i], i + 2);
    }

    // Load View, Projection Matrix and Viewport settings
    camera->SetViewPort();
    m_shader->SetViewAndProj(camera);
    m_shader->SetVec3("viewPos", camera->position);
}

void LightningRenderer::Render(const std::vector<Entity>& entities, const std::unique_ptr<Material>& material, const Geometry* geometry) {
    // Prepare Geometry (Bind VAO)
    geometry->Bind();

    // Prepare Texture and Material
    m_shader->SetFloat("shininess", material->shininess);
    if (material->texture == nullptr) {
        m_shader->SetBool("useTexture", false);
        m_shader->SetVec3("objectColor", material->color);
    } else {
        m_shader->SetBool("useTexture", true);
        m_shader->SetVec3("objectColor", glm::vec3(0.0f));
        material->texture->Bind(GL_TEXTURE0);
    }

    for (const auto& entity : entities) {
        // Prepare Instance
        glm::mat4 model_matrix = entity.GetModelMatrix();
        m_shader->SetMat4("model", model_matrix);

        // Draw
        geometry->DrawOnly();
    }

    // Unbind VAO and Texture
    geometry->UnBind();
    material->texture->UnBind();
}