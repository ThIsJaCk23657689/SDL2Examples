#include "Renderer/EntitiesRenderer.hpp"
#include "State.hpp"

EntitiesRenderer::EntitiesRenderer(LightningShader * shader) :
    lightning_shader(shader) {
    // Run this out of loop

    // 設定 gl
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void EntitiesRenderer::Initialize() {
    // Run this every frame

    // 是否開啟 Back Face Culling
    if (state.world->culling) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }

    // 是否開啟畫線模式
    if (state.world->wire_mode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // 清除快取
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EntitiesRenderer::Render(const std::unique_ptr<Entity>& entity) {
    glm::mat4 model_matrix = entity->GetModelMatrix();

    lightning_shader->SetMat4("model", model_matrix);

    entity->model->texture->Bind();
    entity->model->geometry->Draw();
    entity->model->texture->UnBind();
}