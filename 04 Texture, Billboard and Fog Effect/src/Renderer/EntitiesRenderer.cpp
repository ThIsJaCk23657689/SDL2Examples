#include "Renderer/EntitiesRenderer.hpp"

void EntitiesRenderer::Initialize() {

}

void EntitiesRenderer::Render(Entity * const entity, Shader * const shader) {
    glm::mat4 model_matrix = entity->GetModelMatrix();

    shader->SetMat4("model", model_matrix);

    entity->model->texture->Bind();
    entity->model->geometry->Draw();
    entity->model->texture->UnBind();
}