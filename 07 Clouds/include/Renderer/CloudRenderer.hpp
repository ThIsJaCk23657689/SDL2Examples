#ifndef CLOUDRENDERER_HPP
#define CLOUDRENDERER_HPP

#include "Renderer/Renderer.hpp"
#include "Shader/CloudShader.hpp"

class Cloud;

class CloudRenderer : public Renderer {
public:
    CloudRenderer(CloudShader* shader);
    ~CloudRenderer() = default;

    virtual void Prepare(const Camera* camera) override;
    void Render(const Cloud* cloud);

private:
    CloudShader* m_shader;

};

#endif
