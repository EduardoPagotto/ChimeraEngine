#pragma once
namespace ce {
class IRenderable2D {
  public:
    virtual ~IRenderable2D() = default;
    virtual void submit(class IRenderer2D& renderer) = 0;
};
} // namespace ce
