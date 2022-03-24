#ifndef SCREENRENDERER_HPP
#define SCREENRENDERER_HPP

#include "Geometry/2D/Screen.hpp"
#include "Texture/Texture2D.hpp"
#include "Shader/ScreenShader.hpp"

struct ScreenRenderer {
    ScreenRenderer(ScreenShader* shader);
    void Prepare();
    void Render(const Texture2D* screen_texture, const Texture2D* bloom_texture, const Screen* screen);

private:
    ScreenShader* m_shader;
};

#endif
