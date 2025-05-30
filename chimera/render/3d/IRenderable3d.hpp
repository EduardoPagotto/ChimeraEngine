#pragma once
namespace ce {
class IRenderable3d {
  public:
    virtual ~IRenderable3d() = default;
    virtual void submit(struct RenderCommand& command, class IRenderer3d& renderer) = 0;
};
} // namespace ce
