#ifndef __CHIMERA_SIMPLE_RENDER_3D_HPP
#define __CHIMERA_SIMPLE_RENDER_3D_HPP

#include "IRenderer3d.hpp"
#include <deque>

namespace Chimera {

class SimpleRender3d : public IRenderer3d {
  public:
    virtual void begin(glm::vec3* eye, Core::Frustum* frustrun, bool logData) override;
    virtual void end() override;
    virtual void submit(IRenderable* renderable) override;
    virtual void flush() override;

  private:
    std::deque<IRenderable*> renderQueue;
    bool logData;
    Core::Frustum* frustrun;
    glm::vec3* eye;

    uint32_t totIBO;
    uint32_t totFaces;
};

} // namespace Chimera
#endif