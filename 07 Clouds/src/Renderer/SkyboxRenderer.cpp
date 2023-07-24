#include "Renderer/SkyboxRenderer.hpp"

#include "State.hpp"

SkyboxRenderer::SkyboxRenderer(SkyboxShader* shader) : m_shader(shader) {

}

void SkyboxRenderer::Prepare(const Camera* camera) {
    m_shader->Start();
    m_shader->SetInt("skybox", 0);
    m_shader->SetFloat("bloomThreshold", state.world->bloom_threshold);

    // Load View and Projection Matrix
    glm::mat4 view = glm::mat4(glm::mat3(camera->View()));
    glm::mat4 projection = camera->Projection();
    m_shader->SetMat4("view", view);
    m_shader->SetMat4("projection", projection);
}

void SkyboxRenderer::Render(const CubeMap* cubemap, const Geometry* geometry) {
    cubemap->Active(GL_TEXTURE0);
    cubemap->Bind();
    geometry->Draw();
}