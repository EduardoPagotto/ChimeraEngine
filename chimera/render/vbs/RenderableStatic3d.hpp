#ifndef __CHIMERA_RENDERABLESTATIC3D__HPP
#define __CHIMERA_RENDERABLESTATIC3D__HPP

#include "IRenderable.hpp"
#include <deque>

namespace Chimera {

class SimpleRender3d : public IRender3d {
  public:
    virtual void submit(IRenderable* renderable) override;
    virtual void flush() override;

  private:
    std::deque<IRenderable*> renderQueue;
};

} // namespace Chimera
#endif