#include "Renderer/ScreenRenderer.hpp"

ScreenRenderer::ScreenRenderer(ScreenShader* shader) : m_shader(shader) {

}

void ScreenRenderer::Prepare() {
    m_shader->Start();
    m_shader->SetInt("screenTexture", 0);
}

void ScreenRenderer::Render(const Texture2D* texture, const Screen* screen) {
    texture->Bind(GL_TEXTURE0);
    screen->Draw();
}