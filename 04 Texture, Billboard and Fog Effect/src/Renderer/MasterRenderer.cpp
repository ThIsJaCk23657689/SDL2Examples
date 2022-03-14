#include "Renderer/MasterRenderer.hpp"

MasterRenderer::MasterRenderer() {
    // Game Class 初始化的時候就會執行

    // 建立 Shaders
    basic_shader = std::make_unique<BasicShader>();
    lightning_shader = std::make_unique<LightningShader>();
    alpha_shader = std::make_unique<AlphaShader>();

    // 建立 Renderer
    basic_renderer = std::make_unique<BasicRenderer>(basic_shader.get());
    lightning_renderer = std::make_unique<LightningRenderer>(lightning_shader.get());
    alpha_renderer = std::make_unique<AlphaRenderer>(alpha_shader.get());

    // 設定 gl
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MasterRenderer::Initialize() {
    // 在每一次的 Game loop 都會執行，且在分割畫面之前

    // 是否開啟 Back Face Culling
    if (state.world->culling) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }

    // 清除快取
    if (state.world->fog_bind_bg_color) {
        glClearColor(state.world->my_fog->color.r, state.world->my_fog->color.g, state.world->my_fog->color.b, 1.0f);
    } else {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MasterRenderer::Render(const std::unique_ptr<Camera>& camera) {
    // Viewport settings
    camera->SetViewPort();

    // 繪製需要光照的物體 (lightning renderer)
    lightning_renderer->Prepare(camera);
    lightning_renderer->Render(state.world->sun, state.world->my_sphere.get());
    lightning_renderer->Render(state.world->earth, state.world->my_sphere.get());
    lightning_renderer->Render(state.world->moon, state.world->my_sphere.get());
    lightning_renderer->Render(state.world->rick_rolls, state.world->my_cube.get());
    lightning_renderer->Render(state.world->ground, state.world->my_cube.get());

    lightning_renderer->Render(state.world->camera, state.world->my_cube.get());

    // TODO:: 攝影機的繪製，可以考慮讓 Camera 繼承 Entity，不過要解決一個問題： 如果我只有一個 Entity 要繪製，那還需要為此建立一個 vector 嗎，是不是有點太浪費資源。
    // 有沒有辦法使用 Entity.Draw(Renderer) 的方式?
    // 但這樣又會本末倒置，不然就是說要採用 Renderer.Render(Entity, Material, Geometry) 多載的方式，嘗試看看。


    // 繪製光球 這邊設計應該可以再更好
    basic_renderer->Prepare(camera);
    if (state.world->my_directional_light->enable) {
        basic_renderer->Render(state.world->my_directional_light->entity, state.world->my_sphere.get());
    }
    for (auto& my_point_light : state.world->my_point_lights) {
        if (my_point_light->enable) {
            basic_renderer->Render(my_point_light->entity, state.world->my_sphere.get());
        }
    }


}

void MasterRenderer::Destroy() {
    basic_shader->Destroy();
    lightning_shader->Destroy();
    alpha_shader->Destroy();
}