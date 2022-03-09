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

void EntitiesRenderer::Render(const std::unordered_map<Model, std::vector<Entity>>& entities) {
    for (const auto& iter : entities) {
        auto *model = &iter.first;

        // Prepare Geometry (Bind VAO)
        model->geometry->Bind();

        // Prepare Texture and Material
        lightning_shader->SetFloat("shininess", state.world->shininess);
        if (model->texture == nullptr) {
            lightning_shader->SetBool("useTexture", false);
            lightning_shader->SetVec3("objectColor", glm::vec3(42 / 255.0f, 219 / 255.0f, 89 / 255.0f));
        } else {
            lightning_shader->SetBool("useTexture", true);
            lightning_shader->SetVec3("objectColor", glm::vec3(0.0f));
            model->texture->Bind(GL_TEXTURE0);
        }

        auto batch = iter.second;
        for(auto& entity : batch) {
            // Prepare Instance
            glm::mat4 model_matrix = entity.GetModelMatrix();
            lightning_shader->SetMat4("model", model_matrix);

            // Draw
            model->geometry->DrawOnly();
        }

        // Unbind VAO and Texture
        model->geometry->UnBind();
        model->texture->UnBind();
    }
}