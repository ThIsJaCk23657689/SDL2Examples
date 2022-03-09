#include "Renderer/MasterRenderer.hpp"

#include "State.hpp"

void MasterRenderer::Initialize() {
    lightning_shader = std::make_unique<LightningShader>();
    entities_renderer = std::make_unique<EntitiesRenderer>(lightning_shader.get());
}

void MasterRenderer::Render(const std::unique_ptr<Camera>& camera) {
    entities_renderer->Initialize();
    lightning_shader->Start();
    lightning_shader->SetBool("useLighting", true);
    lightning_shader->SetBool("useBlinnPhong", true);
    lightning_shader->SetVec3("objectColor", glm::vec3(0.0f));
    lightning_shader->SetBool("useTexture", false);
    lightning_shader->SetInt("diffuse_texture", 0);

    // Load Lights
    lightning_shader->SetLight(state.world->my_directional_light, 0);
    lightning_shader->SetLight(state.world->my_spotlight, 1);
    for (int i = 0; i < state.world->my_point_lights.size(); ++i) {
        lightning_shader->SetLight(state.world->my_point_lights[i], i + 2);
    }

    // Load View, Projection Matrix and Viewport settings
    camera->SetViewPort();
    lightning_shader->SetViewAndProj(camera);
    lightning_shader->SetVec3("viewPos", camera->position);

    // Render objects
    entities_renderer->Render(entities);

    lightning_shader->Stop();
    entities.clear();
}

void MasterRenderer::ProcessEntity(const Entity& entity) {
    auto model = *entity.model;

    if (entities.find(model) != entities.end()) {
        std::vector<Entity> batch = entities[model];
    } else {
        std::vector<Entity> new_batch;
        new_batch.push_back(entity);
        entities[model] = new_batch;
    }
}

void MasterRenderer::Destroy() {
    lightning_shader->Destroy();
}