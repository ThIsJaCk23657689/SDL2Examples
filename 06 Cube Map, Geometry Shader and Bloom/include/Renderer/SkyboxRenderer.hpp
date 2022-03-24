#ifndef SKYBOXRENDERER_HPP
#define SKYBOXRENDERER_HPP

#include "Texture/CubeMap.hpp"
#include "Renderer/Renderer.hpp"
#include "Shader/SkyboxShader.hpp"

struct SkyboxRenderer : public Renderer {
    SkyboxRenderer(SkyboxShader* shader);
    void Prepare(const std::unique_ptr<Camera>& camera) override;
    void Render(const CubeMap* cubemap, const Geometry* geometry);

private:
    SkyboxShader* m_shader;

};

#endif
