#include "Renderer/ScreenRenderer.hpp"

#include "State.hpp"

ScreenRenderer::ScreenRenderer(ScreenShader* shader) : m_shader(shader) {

}

void ScreenRenderer::Prepare() {
    m_shader->Start();
    m_shader->SetInt("screenTexture", 0);
    m_shader->SetInt("bloomTexture", 1);
    m_shader->SetInt("screenMode", state.world->current_screen_mode);

    m_shader->SetBool("useGamma", state.world->use_gamma_correction);
    m_shader->SetFloat("gammaValue", state.world->gamma_value);

    m_shader->SetBool("useHDR", state.world->use_hdr);
    m_shader->SetInt("hdrMode", state.world->current_hdr_mode);
    m_shader->SetFloat("hdrExposure", state.world->hdr_exposure);
}

void ScreenRenderer::Render(const Texture2D* screen_texture, const Texture2D* bloom_texture, const Screen* screen) {
    screen_texture->Bind(GL_TEXTURE0);
    bloom_texture->Bind(GL_TEXTURE1);
    screen->Draw();
}