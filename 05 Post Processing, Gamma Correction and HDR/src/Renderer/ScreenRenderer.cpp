#include "Renderer/ScreenRenderer.hpp"

#include "State.hpp"

ScreenRenderer::ScreenRenderer(ScreenShader* shader) : m_shader(shader) {

}

void ScreenRenderer::Prepare() {
    m_shader->Start();
    m_shader->SetInt("screenTexture", 0);
    m_shader->SetInt("screenMode", state.world->current_screen_mode);
    m_shader->SetBool("useGamma", state.world->use_gamma_correction);
    m_shader->SetFloat("gammaValue", state.world->gamma_value);

}

void ScreenRenderer::Render(const Texture2D* texture, const Screen* screen) {
    texture->Bind(GL_TEXTURE0);
    screen->Draw();
}