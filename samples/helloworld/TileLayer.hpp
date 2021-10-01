#ifndef __HELLOWORD_TILELAYER__HPP
#define __HELLOWORD_TILELAYER__HPP

#include "chimera/render/2d/BatchRender2D.hpp"
#include "chimera/render/2d/layer/Layer.hpp"
#include <glm/gtx/transform.hpp>

class TileLayer : public Chimera::Layer {
  public:
    TileLayer(Chimera::Shader* shader)
        : Chimera::Layer(new Chimera::BatchRender2D(), shader, glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)) {}
    virtual ~TileLayer() {}
};
#endif