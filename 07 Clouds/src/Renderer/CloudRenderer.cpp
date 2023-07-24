#include "Renderer/CloudRenderer.hpp"
#include "Model/Cloud.hpp"
#include "Texture/Texture3D.hpp"

CloudRenderer::CloudRenderer(CloudShader* shader) : m_shader(shader) {

}

void CloudRenderer::Prepare(const Camera* camera) {
    m_shader->Start();
    m_shader->SetVec3("g_ObjectColor", glm::vec3(0.0f));

    // Load Lights
//    m_shader->SetLight(state.world->my_directional_light, 0);
//    m_shader->SetLight(state.world->my_spotlight, 1);
//    for (int i = 0; i < state.world->my_point_lights.size(); ++i) {
//        m_shader->SetLight(state.world->my_point_lights[i], i + 2);
//    }

    m_shader->SetInt("perlinNoiseTexture", 0);

    // Load View and Projection Matrix
    m_shader->SetViewAndProj(camera);
    m_shader->SetVec3("g_ViewPos", camera->position);
}

void CloudRenderer::Render(const Cloud* cloud) {
    // Prepare Geometry (Bind VAO)
    cloud->Bind();

    cloud->GetPerlinNoiseTexture3D()->Active(GL_TEXTURE0);
    cloud->GetPerlinNoiseTexture3D()->Bind();

    // Prepare Texture and Material
    // m_shader->SetFloat("shininess", entity.material.shininess);
    m_shader->SetVec3("g_ObjectColor", cloud->GetColor());
    m_shader->SetVec3("g_ObjectPosition", cloud->GetPosition());
    m_shader->SetVec3("g_ObjectSize", cloud->GetSize());
    m_shader->SetInt("g_SampleCount", cloud->SampleCount);
    m_shader->SetFloat("g_Absorbance", cloud->Absorbance);

    // Prepare Instance
    const auto& model_matrix = cloud->GetModelMatrix();
    m_shader->SetMat4("model", model_matrix);

    // Draw
    cloud->DrawOnly();

    // Unbind VAO and Texture
    cloud->UnBind();
    // entity.material.texture->UnBind();
}