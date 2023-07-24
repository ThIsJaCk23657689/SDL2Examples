#include "Renderer/LightningRenderer.hpp"

#include "Texture/TextureManager.hpp"

LightningRenderer::LightningRenderer(LightningShader* shader) : m_shader(shader) {
    
}

void LightningRenderer::Prepare(const Camera* camera) {
    m_shader->Start();
    m_shader->SetBool("useLighting", true);
    m_shader->SetBool("useBlinnPhong", true);
    m_shader->SetVec3("objectColor", glm::vec3(0.0f));
    m_shader->SetBool("useTexture", false);
    m_shader->SetBool("emissionTexture", false);
    m_shader->SetInt("diffuse_texture", 0);
    // m_shader->SetInt("skybox", 1);

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
        // 這個會有一個很大的問題，如果說每個 entity 都是使用同一個 texture 的話，這樣的無疑會造成效能上的浪費（因為一直 bind texture）
        // 但如果把綁定 texture 寫在外層，這樣又會卡死說同一個 entities 只能使用一樣的 texture。
        // 不同 entities 有不同的材質，不停切換也會比較傷效能，一個提昇優化的方法就是使用 Texture Atlas。
        // 但現階段先不追究那麼多。
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