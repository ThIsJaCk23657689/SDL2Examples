#include "Renderer/GaussianBlurRenderer.hpp"

#include "State.hpp"
#include <glad/glad.h>

GaussianBlurRenderer::GaussianBlurRenderer(GaussianBlurShader* shader) : m_shader(shader) {
}

void GaussianBlurRenderer::Prepare(bool is_horizontal) {
    m_shader->Start();
    m_shader->SetInt("image", 0);
    m_shader->SetBool("isHorizontal", is_horizontal);
}

void GaussianBlurRenderer::Render(const Texture2D* image, const Screen* screen) {
    image->Bind(GL_TEXTURE0);
    screen->Draw();
}