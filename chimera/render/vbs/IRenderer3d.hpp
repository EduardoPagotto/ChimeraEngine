#pragma once

namespace Chimera {

class IRenderer3d {
  public:
    virtual void begin(class ICamera* camera) = 0;
    virtual void submit(class IRenderable* renderable) = 0;
    virtual void end() = 0;
    virtual void flush() = 0;
};
} // namespace Chimera
