#ifndef ALPHARENDERER_HPP
#define ALPHARENDERER_HPP

#include "Renderer/Renderer.hpp"
#include "Shader/AlphaShader.hpp"

struct ViewVolumeRenderer : public Renderer {
    explicit ViewVolumeRenderer(AlphaShader* shader);
    virtual void Prepare(const Camera* camera) override;
    void Render(const Geometry* geometry);

private:
    AlphaShader* m_shader;
};

#endif
