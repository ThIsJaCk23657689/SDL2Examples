#ifndef ENTITIESRENDERER_HPP
#define ENTITIESRENDERER_HPP

#include <memory>

#include "Renderer.hpp"
#include "Shader/LightningShader.hpp"

struct EntitiesRenderer : public Renderer {
    EntitiesRenderer(LightningShader * shader);
    void Initialize() override;
    void Render(const std::unique_ptr<Entity>& entity) override;

private:
    LightningShader* lightning_shader;

};

#endif
