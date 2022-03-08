#include "Renderer/MasterRenderer.hpp"

#include "State.hpp"

void MasterRenderer::Initialize() {

}

void MasterRenderer::Render(const std::unique_ptr<Camera>& camera) {
    entities_renderer->Initialize();
    lightning_shader->Start();

    // Load Lights
    lightning_shader->SetLight(state.world->my_directional_light, 0);
    lightning_shader->SetLight(state.world->my_spotlight, 1);
    for (int i = 0; i < state.world->my_point_lights.size(); ++i) {
        lightning_shader->SetLight(state.world->my_point_lights[i], i + 2);
    }

    // Load View, Projection Matrix and Viewport settings
    camera->SetViewPort();
    lightning_shader->SetViewAndProj(camera);

    //

    lightning_shader->Stop();
    entities.clear();
}

void MasterRenderer::Destroy() {
    lightning_shader->Destroy();
}