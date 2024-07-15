#pragma once
#include "Layer.hpp"

namespace Chimera {
class Tile : public Layer {
  public:
    Tile(const std::string& name, IRenderer2D* renderer, Shader shader, Camera* camera);
    virtual ~Tile();
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onUpdate(IViewProjection& vp, const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual void onRender() override;
};
} // namespace Chimera