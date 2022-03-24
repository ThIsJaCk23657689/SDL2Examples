#ifndef GAUSSIANBLURRENDERER_HPP
#define GAUSSIANBLURRENDERER_HPP

#include "Geometry/2D/Screen.hpp"
#include "Texture/Texture2D.hpp"
#include "Shader/GaussianBlurShader.hpp"

struct GaussianBlurRenderer {
    GaussianBlurRenderer(GaussianBlurShader* shader);
    void Prepare(bool is_horizontal);
    void Render(const Texture2D* image, const Screen* screen);

private:
    GaussianBlurShader* m_shader;

};

#endif
