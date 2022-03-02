#ifndef RENDERER_HPP
#define RENDERER_HPP

//#include "Model/Model.hpp"
#include "Shader/Shader.hpp"
#include "World/Entity.hpp"

struct Renderer {
    virtual void Initialize() = 0;
    virtual void Render(Entity * const entity, Shader * const shader) = 0;
};

#endif
