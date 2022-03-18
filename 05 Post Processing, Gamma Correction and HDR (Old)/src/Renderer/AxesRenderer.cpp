#include "Renderer/AxesRenderer.hpp"

AxesRenderer::AxesRenderer(BasicShader* shader) : m_shader(shader) {
    model = std::make_unique<MatrixStack>();
}

void AxesRenderer::Prepare(const std::unique_ptr<Camera>& camera) {
    m_shader->Start();
    m_shader->SetVec3("objectColor", glm::vec3(0.0f));

    // Load Fog
    state.world->my_fog->Set(reinterpret_cast<std::unique_ptr<Shader>&>(m_shader));

    // Load View and Projection Matrix
    m_shader->SetViewAndProj(camera);
    m_shader->SetVec3("viewPos", camera->position);
}

void AxesRenderer::Render(float length) {
    model->Push();

    // 記得先抓取該 Entity 的 Position and Rotation 資訊
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model->Save(model_matrix);

    // Draw
    DrawAxes(length);

    model->Pop();
}

void AxesRenderer::Render(const Entity& entity, float length) {
    model->Push();

    // 記得先抓取該 Entity 的 Position and Rotation 資訊
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model->Save(glm::translate(model->Top(), entity.position));
    model->Save(glm::rotate(model->Top(), glm::radians(-entity.rotate.y), glm::vec3(0.0, 1.0, 0.0)));
    model->Save(glm::rotate(model->Top(), glm::radians(entity.rotate.x), glm::vec3(1.0, 0.0, 0.0)));
    model->Save(glm::rotate(model->Top(), glm::radians(entity.rotate.z), glm::vec3(0.0, 0.0, 1.0)));

    // Draw
    DrawAxes(length);

    model->Pop();
}

void AxesRenderer::DrawAxes(float length) {
    model->Push();
    model->Save(glm::translate(model->Top(), glm::vec3(length / 2.0f, 0.0f, 0.0f)));
    model->Save(glm::scale(model->Top(), glm::vec3(length, length / 20.0f, length / 20.0f)));
    m_shader->SetVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
    m_shader->SetMat4("model", model->Top());
    state.world->my_cube->Draw();
    model->Pop();

    model->Push();
    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, length / 2.0f, 0.0f)));
    model->Save(glm::scale(model->Top(), glm::vec3(length / 20.0f, length, length / 20.0f)));
    m_shader->SetVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f));
    m_shader->SetMat4("model", model->Top());
    state.world->my_cube->Draw();
    model->Pop();

    model->Push();
    model->Save(glm::translate(model->Top(), glm::vec3(0.0f, 0.0f, length / 2.0f)));
    model->Save(glm::scale(model->Top(), glm::vec3(length / 20.0f, length / 20.0f, length)));
    m_shader->SetVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));
    m_shader->SetMat4("model", model->Top());
    state.world->my_cube->Draw();
    model->Pop();
}