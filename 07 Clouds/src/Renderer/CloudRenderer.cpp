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

    m_shader->SetVec3("g_NoiseOffset", cloud->NoiseOffset);
    m_shader->SetFloat("g_NoiseScale", cloud->NoiseScale);
    m_shader->SetFloat("g_DensityThreshold", cloud->DensityThreshold);
    m_shader->SetFloat("g_DensityMultiplier", cloud->DensityMultiplier);

    const auto& light = state.world->my_point_lights[5];
    m_shader->SetVec3("g_Light.position", light->entity.position);
    m_shader->SetVec3("g_Light.color", light->diffuse);

    m_shader->SetFloat("g_Light.constant", 1.0f);
    m_shader->SetFloat("g_Light.linear", 0.35f);
    m_shader->SetFloat("g_Light.quadratic", 0.44f);

    m_shader->SetBool("g_Light.enable", light->enable);
    m_shader->SetFloat("g_DarknessThreshold", cloud->DarknessThreshold);
    m_shader->SetFloat("g_PhaseParameter1", cloud->PhaseParameter1);
    m_shader->SetFloat("g_PhaseParameter2", cloud->PhaseParameter2);
    m_shader->SetBool("g_UseAnisotropic", cloud->UseAnisotropic);


    // Prepare Instance
    const auto& model_matrix = cloud->GetModelMatrix();
    m_shader->SetMat4("model", model_matrix);

    // Draw
    cloud->DrawOnly();

    // Unbind VAO and Texture
    cloud->UnBind();
    // entity.material.texture->UnBind();
}