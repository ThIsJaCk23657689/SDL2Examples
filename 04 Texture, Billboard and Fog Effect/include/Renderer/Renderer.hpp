#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <map>
#include <memory>

#include "Shader/Shader.hpp"
#include "World/Entity.hpp"
#include "Model/Model.hpp"

struct Renderer {
    Renderer();
    virtual void Initialize() = 0;
    virtual void Render(const std::unordered_map<Model, std::vector<Entity>>& entities) = 0;
};

#endif
