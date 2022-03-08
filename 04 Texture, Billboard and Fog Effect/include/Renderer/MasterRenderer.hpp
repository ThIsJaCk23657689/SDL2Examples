#ifndef MASTERRENDERER_HPP
#define MASTERRENDERER_HPP

#include <map>
#include <memory>
#include <vector>

#include "Camera.hpp"
#include "Shader/LightningShader.hpp"
#include "Renderer/EntitiesRenderer.hpp"
#include "Model/Model.hpp"
#include "World/Entity.hpp"

struct MasterRenderer {
    void Initialize();
    void Render(const std::unique_ptr<Camera>& camera);

    void Destroy();

private:
    std::unique_ptr<LightningShader> lightning_shader = nullptr;
    std::unique_ptr<EntitiesRenderer> entities_renderer = nullptr;
    std::map<Model, std::vector<Entity>> entities;

};

#endif