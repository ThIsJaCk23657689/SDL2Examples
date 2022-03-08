#ifndef MASTERRENDERER_HPP
#define MASTERRENDERER_HPP

#include "Renderer.hpp"

struct MasterRenderer : public Renderer {
    void Initialize() override;
    void Render(const std::unique_ptr<Entity>& entity, const std::unique_ptr<Shader>& shader) override;
};

#endif