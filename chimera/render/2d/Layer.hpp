#pragma once
#include "Renderable2D.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/core/visible/Shader.hpp"

namespace ce {

class Layer : public IStateMachine {
  public:
    Layer(IRenderer2D* renderer, std::shared_ptr<Shader> shader, Camera* camera, const std::string& name);
    virtual ~Layer();
    virtual void onRender() override;
    inline void add(IRenderable2D* renderable) { renderables.push_back(renderable); }

    Camera* getCamera() const { return camera; };

  protected:
    std::shared_ptr<Shader> shader;
    Camera* camera;

  private:
    IRenderer2D* renderer;
    std::vector<IRenderable2D*> renderables;
};
} // namespace ce
