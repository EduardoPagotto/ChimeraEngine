#pragma once

namespace Chimera {
class IRenderable3d {
  public:
    virtual ~IRenderable3d() = default;
    virtual void submit(class RenderCommand& command, class IRenderer3d& renderer) = 0;
};
} // namespace Chimera
