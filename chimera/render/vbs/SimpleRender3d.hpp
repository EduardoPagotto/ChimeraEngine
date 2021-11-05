#pragma once

#include "IRenderer3d.hpp"
#include "chimera/render/ICamera.hpp"
#include "chimera/render/partition/Frustum.hpp"
#include <cstdint>
#include <deque>

namespace Chimera {

class SimpleRender3d : public IRenderer3d {
  public:
    SimpleRender3d() : logData(false), camera(nullptr), totIBO(0), totFaces(0) {}
    virtual ~SimpleRender3d() {}

    virtual void begin(ICamera* camera) override;
    virtual void submit(IRenderable* renderable) override;
    virtual void end() override;
    virtual void flush() override;

    void logToggle() { logData = !logData; }
    bool getLog() const { return logData; }

  private:
    std::deque<IRenderable*> renderQueue;
    ICamera* camera;
    Frustum frustum;

    bool logData;
    uint32_t totIBO;
    uint32_t totFaces;
};
} // namespace Chimera
