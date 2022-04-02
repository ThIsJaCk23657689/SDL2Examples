#ifndef NORMALMAPRENDERER_HPP
#define NORMALMAPRENDERER_HPP

#include "Camera.hpp"
#include "Renderer/Renderer.hpp"
#include "Shader/NormalMapShader.hpp"
#include "Texture/Texture2D.hpp"

struct NormalMapRenderer : public Renderer {
    NormalMapRenderer(NormalMapShader* shader);
    void Prepare(const std::unique_ptr<Camera>& camera) override;
    void Render(const Entity& entity, const Geometry* geometry, const Texture2D* normal_texture);

private:
    NormalMapShader* m_shader;

};

#endif
