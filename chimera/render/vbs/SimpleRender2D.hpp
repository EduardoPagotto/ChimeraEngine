#ifndef __CHIMERA_SIMPLERENDERER_2D__HPP
#define __CHIMERA_SIMPLERENDERER_2D__HPP

#include "Renderable2D.hpp"

namespace Chimera {

class SimpleRender2D : public IRenderer2D {
  public:
    virtual void submit(IRenderable2D* renderable) override;
    virtual void flush() override;

  private:
    std::deque<IRenderable2D*> renderQueue;
};
} // namespace Chimera
#endif