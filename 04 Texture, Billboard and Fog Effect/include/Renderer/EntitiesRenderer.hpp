#ifndef ENTITIESRENDERER_HPP
#define ENTITIESRENDERER_HPP

#include "Renderer.hpp"

struct EntitiesRenderer : public Renderer {
    void Initialize() override;
    void Render(Entity * const entity, Shader * const shader) override;
};

#endif
