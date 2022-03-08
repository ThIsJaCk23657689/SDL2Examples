#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

#include "Shader/Shader.hpp"
#include "World/Entity.hpp"

struct Renderer {
    Renderer();
    virtual void Initialize() = 0;
    virtual void Render(const std::unique_ptr<Entity>& entity, const std::unique_ptr<Shader>& shader) = 0;
};

#endif
