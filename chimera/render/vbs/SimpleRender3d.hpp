#ifndef __CHIMERA_SIMPLE_RENDER_3D_HPP
#define __CHIMERA_SIMPLE_RENDER_3D_HPP

#include "IRenderer3d.hpp"
#include <deque>

namespace Chimera {

class SimpleRender3d : public IRenderer3d {
  public:
    SimpleRender3d() : logData(false), frustrun(nullptr), totIBO(0), totFaces(0) {}
    virtual ~SimpleRender3d() {}

    virtual void begin(Frustum* frustrun) override;
    virtual void submit(IRenderable* renderable) override;
    virtual void end() override;
    virtual void flush() override;

    void logToggle() { logData = !logData; }
    bool getLog() const { return logData; }

  private:
    std::deque<IRenderable*> renderQueue;
    Frustum* frustrun;

    bool logData;
    uint32_t totIBO;
    uint32_t totFaces;
};

} // namespace Chimera
#endif