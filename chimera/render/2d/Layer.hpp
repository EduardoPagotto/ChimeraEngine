#ifndef __CHIMERA_IRENDERABLE__HPP
#define __CHIMERA_IRENDERABLE__HPP

#include "Renderable2D.hpp"

namespace Chimera {

class Layer {
  public:
    virtual ~Layer();
    virtual void add(IRenderable2D* renderable);
    void render();

  protected:
    Layer(IRenderer2D* renderer, Shader* shader, glm::mat4 projectionMatrix);
    IRenderer2D* renderer;
    std::vector<IRenderable2D*> renderables;
    Shader* shader;
    glm::mat4 projectionMatrix;
};

class TileLayer : public Layer {
  public:
    TileLayer(Shader* shader);
    virtual ~TileLayer();
};
} // namespace Chimera
#endif