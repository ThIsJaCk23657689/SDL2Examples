#include "Renderer/LightningRenderer.hpp"

#include "Texture/TextureManager.hpp"

LightningRenderer::LightningRenderer(LightningShader* shader) : m_shader(shader) {
    
}

void LightningRenderer::Prepare(const std::unique_ptr<Camera>& camera) {
    m_shader->Start();
    m_shader->SetBool("useLighting", true);
    m_shader->SetBool("useBlinnPhong", true);
    m_shader->SetVec3("objectColor", glm::vec3(0.0f));
    m_shader->SetBool("useTexture", false);
    m_shader->SetBool("emissionTexture", false);
    m_shader->SetInt("diffuse_texture", 0);
    m_shader->SetInt("skybox", 1);

    m_shader->SetFloat("bloomThreshold", state.world->bloom_threshold);

    // Load Lights
    m_shader->SetLight(state.world->my_directional_light, 0);
    m_shader->SetLight(state.world->my_spotlight, 1);
    for (int i = 0; i < state.world->my_point_lights.size(); ++i) {
        m_shader->SetLight(state.world->my_point_lights[i], i + 2);
    }

    // Load Fog
    state.world->my_fog->Set(reinterpret_cast<std::unique_ptr<Shader>&>(m_shader));

    // Load View and Projection Matrix
    m_shader->SetViewAndProj(camera);
    m_shader->SetVec3("viewPos", camera->position);
}

void LightningRenderer::Render(const Entity& entity, const Geometry* geometry) {
    // Prepare Geometry (Bind VAO)
    geometry->Bind();

    // For Reflection
    CubeMap skybox = TextureManager::GetCubeMap("SkyBox");
    skybox.Active(GL_TEXTURE1);
    skybox.Bind();

    // Prepare Texture and Material
    m_shader->SetFloat("shininess", entity.material.shininess);
    if (entity.material.texture == nullptr) {
        m_shader->SetBool("useTexture", false);
        m_shader->SetVec3("objectColor", entity.material.color);
    } else {
        if (entity.material.emission_texture) {
            m_shader->SetBool("emissionTexture", true);
        } else {
            m_shader->SetBool("emissionTexture", false);
        }
        m_shader->SetBool("useTexture", true);
        m_shader->SetVec3("objectColor", glm::vec3(0.0f));
        entity.material.texture->Bind(GL_TEXTURE0);
    }

    // Prepare Instance
    glm::mat4 model_matrix = entity.GetModelMatrix();
    m_shader->SetMat4("model", model_matrix);

    // Draw
    geometry->DrawOnly();

    // Unbind VAO and Texture
    geometry->UnBind();
    entity.material.texture->UnBind();
}

void LightningRenderer::Render(const std::vector<Entity>& entities, const Geometry* geometry) {
    // Prepare Geometry (Bind VAO)
    geometry->Bind();

    for (const auto& entity : entities) {
        // Prepare Texture and Material
        // ??????????????????????????????????????????????????? entity ????????????????????? texture ?????????????????????????????????????????????????????????????????? bind texture???
        // ?????????????????? texture ????????????????????????????????????????????? entities ????????????????????? texture???
        // ?????? entities ???????????????????????????????????????????????????????????????????????????????????????????????? Texture Atlas???
        // ????????????????????????????????????
        m_shader->SetFloat("shininess", entity.material.shininess);
        if (entity.material.texture == nullptr) {
            m_shader->SetBool("useTexture", false);
            m_shader->SetVec3("objectColor", entity.material.color);
        } else {
            if (entity.material.emission_texture) {
                m_shader->SetBool("emissionTexture", true);
            } else {
                m_shader->SetBool("emissionTexture", false);
            }
            m_shader->SetBool("useTexture", true);
            m_shader->SetVec3("objectColor", glm::vec3(0.0f));
            entity.material.texture->Bind(GL_TEXTURE0);
        }

        // Prepare Instance
        glm::mat4 model_matrix = entity.GetModelMatrix();
        m_shader->SetMat4("model", model_matrix);

        // Draw
        geometry->DrawOnly();

        entity.material.texture->UnBind();
    }

    // Unbind VAO and Texture
    geometry->UnBind();
}