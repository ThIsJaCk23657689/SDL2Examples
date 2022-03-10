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
    // Renderer 初始化設定，
    virtual void Prepare(const std::unique_ptr<Camera>& camera) = 0;

    // 用來繪製單一個 Entity
    virtual void Render(const Entity& entity, const Geometry* geometry) = 0;

    // 用來繪製多個 Entity 時
    virtual void Render(const std::vector<Entity>& entities, const Geometry* geometry) = 0;
};

#endif
