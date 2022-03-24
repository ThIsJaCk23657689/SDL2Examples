#include "Renderer/MasterRenderer.hpp"

#include "Texture/TextureManager.hpp"

MasterRenderer::MasterRenderer() {
    // Game Class 初始化的時候就會執行

    // 建立 Shaders
    basic_shader = std::make_unique<BasicShader>();
    lightning_shader = std::make_unique<LightningShader>();
    alpha_shader = std::make_unique<AlphaShader>();
    screen_shader = std::make_unique<ScreenShader>();
    gaussian_blur_shader = std::make_unique<GaussianBlurShader>();

    // 建立 Renderer
    basic_renderer = std::make_unique<BasicRenderer>(basic_shader.get());
    lightning_renderer = std::make_unique<LightningRenderer>(lightning_shader.get());
    view_volume_renderer = std::make_unique<ViewVolumeRenderer>(alpha_shader.get());
    axes_renderer = std::make_unique<AxesRenderer>(basic_shader.get());
    screen_renderer = std::make_unique<ScreenRenderer>(screen_shader.get());
    gaussian_blur_renderer = std::make_unique<GaussianBlurRenderer>(gaussian_blur_shader.get());

    // 設定 gl
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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void MasterRenderer::Render(const std::unique_ptr<Camera>& camera) {
    // Viewport settings
    camera->SetViewPort();

    // TODO: 或許真的要考慮把形狀跟 renderer 做綁定，不然其實仔細看這個球體也是重覆綁定多次。
    // 繪製需要光照的物體 (lightning renderer)
    lightning_renderer->Prepare(camera);
    lightning_renderer->Render(state.world->sun, state.world->my_sphere.get());
    lightning_renderer->Render(state.world->earth, state.world->my_sphere.get());
    lightning_renderer->Render(state.world->moon, state.world->my_sphere.get());
    lightning_renderer->Render(state.world->rick_rolls, state.world->my_cube.get());
    lightning_renderer->Render(state.world->awesome_faces, state.world->billboard.get());
    lightning_renderer->Render(state.world->ground, state.world->my_floor.get());
    lightning_renderer->Render(state.world->camera, state.world->my_cube.get());

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

    // 繪製 xyz 三軸
    if (state.world->draw_axes) {
        axes_renderer->Prepare(camera);
        axes_renderer->Render(5.0f);
        axes_renderer->Render(state.world->sun);
        axes_renderer->Render(state.world->earth, 5.0f);
        axes_renderer->Render(state.world->moon, 3.0f);
        axes_renderer->Render(state.world->camera, 10.0f);
    }

    // 繪製 View Volume
    view_volume_renderer->Prepare(camera);
    view_volume_renderer->Render(state.world->view_volume.get());
}

void MasterRenderer::Destroy() {
    basic_shader->Destroy();
    lightning_shader->Destroy();
    alpha_shader->Destroy();
}

void MasterRenderer::GaussianBlur(bool is_horizontal, bool first_iteration) {
    gaussian_blur_renderer->Prepare(is_horizontal);
    if (first_iteration) {
        gaussian_blur_renderer->Render(&TextureManager::GetTexture2D("Bloom"), state.world->my_screen.get());
    } else {
        gaussian_blur_renderer->Render(&TextureManager::GetTexture2D("GaussianBlur" + std::to_string(!is_horizontal)), state.world->my_screen.get());
    }
}

void MasterRenderer::RenderScreen(const std::unique_ptr<Camera>& camera) {
    // Call By Application，在每一次 main loop 的結尾執行
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    // Viewport settings
    camera->SetViewPort();

    screen_renderer->Prepare();
    screen_renderer->Render(&TextureManager::GetTexture2D("PostProcessing"), &TextureManager::GetTexture2D("GaussianBlur0"), state.world->my_screen.get());
}