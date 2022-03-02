#ifndef MASTERRENDERER_HPP
#define MASTERRENDERER_HPP

#include "Renderer.hpp"

struct MasterRenderer : public Renderer {
    void Initialize() override;
    void Render(Entity * const entity, Shader * const shader) override;
};

#endif