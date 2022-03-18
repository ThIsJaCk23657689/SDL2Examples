#ifndef AXESRENDERER_HPP
#define AXESRENDERER_HPP

#include "Renderer/Renderer.hpp"
#include "Shader/BasicShader.hpp"
#include "Util/MatrixStack.hpp"

struct AxesRenderer : public Renderer {
    explicit AxesRenderer(BasicShader* shader);
    void Prepare(const std::unique_ptr<Camera>& camera) override;

    void Render(float length = 10.0f);
    void Render(const Entity& entity, float length = 10.0f);

private:
    BasicShader* m_shader;
    std::unique_ptr<MatrixStack> model = nullptr;

    void DrawAxes(float length);
};

#endif
