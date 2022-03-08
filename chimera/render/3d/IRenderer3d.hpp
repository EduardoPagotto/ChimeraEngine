#pragma once
#include <vector>
namespace Chimera {
class IRenderer3d {
  public:
    virtual void begin(class ICamera* camera) = 0;
    virtual void submit(const class RenderCommand& command) = 0;
    virtual void end() = 0;
    virtual void flush() = 0;
    virtual std::vector<class UValue>& uQueue() = 0;
};
} // namespace Chimera
