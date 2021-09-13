#ifndef __CHIMERA_RENDERABLESTATIC3D__HPP
#define __CHIMERA_RENDERABLESTATIC3D__HPP

#include "IRenderable.hpp"
#include <deque>

namespace Chimera {

class SimpleRender3d : public IRender3d {
  public:
    virtual void begin(glm::vec3* eye, Frustum* frustrun, bool logData) override;
    virtual void end() override;
    virtual void submit(IRenderable* renderable) override;
    virtual void flush() override;

  private:
    std::deque<IRenderable*> renderQueue;
    bool logData;
    Frustum* frustrun;
    glm::vec3* eye;
};

} // namespace Chimera
#endif