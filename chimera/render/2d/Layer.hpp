#ifndef __CHIMERA_IRENDERABLE__HPP
#define __CHIMERA_IRENDERABLE__HPP

#include "Renderable2D.hpp"

namespace Chimera {

class Layer {
  protected:
    Layer(IRenderer2D* renderer, Shader* shader, glm::mat4 projectionMatrix);

  public:
    virtual ~Layer();
    inline virtual void add(IRenderable2D* renderable) { renderables.push_back(renderable); }
    void render();

  private:
    Shader* shader;
    IRenderer2D* renderer;
    glm::mat4 projectionMatrix;
    std::vector<IRenderable2D*> renderables;
};
} // namespace Chimera
#endif