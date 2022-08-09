#pragma once
namespace Chimera {
class IRenderable2D {
  public:
    virtual ~IRenderable2D() = default;
    virtual void submit(class IRenderer2D& renderer) = 0;
};
} // namespace Chimera
