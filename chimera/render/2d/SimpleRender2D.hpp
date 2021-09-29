#ifndef __CHIMERA_SIMPLERENDERER_2D__HPP
#define __CHIMERA_SIMPLERENDERER_2D__HPP

#include "Renderable2D.hpp"
#include <deque>

namespace Chimera {

class SimpleRender2D : public IRenderer2D {
  public:
    virtual void begin() override {}
    virtual void submit(IRenderable2D* renderable) override;
    virtual void end() override {}
    virtual void flush() override;
    inline virtual TransformationStack& getStack() override { return stack; };

  private:
    std::deque<IRenderable2D*> renderQueue;
    TransformationStack stack;
};
} // namespace Chimera
#endif