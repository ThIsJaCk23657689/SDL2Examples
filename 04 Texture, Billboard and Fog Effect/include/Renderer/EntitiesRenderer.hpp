#ifndef ENTITIESRENDERER_HPP
#define ENTITIESRENDERER_HPP

#include "Renderer.hpp"

struct EntitiesRenderer : public Renderer {
    EntitiesRenderer(Shader * shader);
    void Initialize() override;
    void Render(const std::unique_ptr<Entity>& entity, const std::unique_ptr<Shader>& shader) override;
};

#endif
