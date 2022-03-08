#include "Renderer/EntitiesRenderer.hpp"

EntitiesRenderer::EntitiesRenderer(Shader * shader) {

}

void EntitiesRenderer::Initialize() {

}

void EntitiesRenderer::Render(const std::unique_ptr<Entity>& entity, const std::unique_ptr<Shader>& shader) {
    glm::mat4 model_matrix = entity->GetModelMatrix();

    shader->SetMat4("model", model_matrix);

    entity->model->texture->Bind();
    entity->model->geometry->Draw();
    entity->model->texture->UnBind();
}