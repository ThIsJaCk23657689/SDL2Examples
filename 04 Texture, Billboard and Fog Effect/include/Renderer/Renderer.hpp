#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <memory>

#include "State.hpp"
#include "Camera.hpp"
#include "World/Entity.hpp"
#include "Material/Material.hpp"
#include "Geometry/Geometry.hpp"

struct Renderer {
    virtual void Prepare(const std::unique_ptr<Camera>& camera) = 0;
    virtual void Render(const std::vector<Entity>& entities, const std::unique_ptr<Material>& material, const Geometry* geometry) = 0;
};

#endif
