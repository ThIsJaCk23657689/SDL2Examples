#ifndef SCREENRENDERER_HPP
#define SCREENRENDERER_HPP

#include "Geometry/2D/Screen.hpp"
#include "Texture/Texture2D.hpp"
#include "Shader/ScreenShader.hpp"

struct ScreenRenderer {
    ScreenRenderer(ScreenShader* shader);
    void Prepare();
    void Render(const Texture2D* texture, const Screen* screen);

private:
    ScreenShader* m_shader;
};

#endif
