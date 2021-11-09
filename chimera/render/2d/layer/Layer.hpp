#pragma once

#include "../Renderable2D.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "chimera/render/CameraOrthographic.hpp"

namespace Chimera {

class Layer : public IStateMachine {
  public:
    Layer(IRenderer2D* renderer, Shader* shader, CameraOrthographic* camera);
    virtual ~Layer();
    inline virtual void add(IRenderable2D* renderable) { renderables.push_back(renderable); }
    virtual void onRender() override;

    CameraOrthographic* getCamera() const { return camera; };

  protected:
    Shader* shader;
    CameraOrthographic* camera;

  private:
    IRenderer2D* renderer;
    std::vector<IRenderable2D*> renderables;
};
} // namespace Chimera
