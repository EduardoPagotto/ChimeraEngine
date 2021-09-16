#ifndef __CHIMERA_IRENDERABLE__HPP
#define __CHIMERA_IRENDERABLE__HPP

#include "IRenderable.hpp"
//#include "SimpleRender3d.hpp"

namespace Chimera {

class Layer {
  public:
    virtual ~Layer();
    virtual void add(IRenderable* renderable);
    void render();

  protected:
    Layer(IRender3d* renderer, Shader* shader, glm::mat4* projectionMatrix);
    IRender3d* renderer;
    std::vector<IRenderable*> renderables;
    Shader* shader;
    glm::mat4 projectionMatrix;
};
} // namespace Chimera
#endif