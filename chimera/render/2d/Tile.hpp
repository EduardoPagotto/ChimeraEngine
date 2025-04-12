#pragma once
#include "Layer.hpp"

namespace ce {
class Tile : public Layer {
  public:
    Tile(const std::string& name, IRenderer2D* renderer, std::shared_ptr<Shader> shader, Camera* camera)
        : Layer(renderer, shader, camera, name) {}
    virtual ~Tile() {}
    virtual void onAttach() override {}
    virtual void onDeatach() override {}
    virtual void onUpdate(IViewProjection& vp, const double& ts) override {}
    virtual bool onEvent(const SDL_Event& event) override { return true; }
    virtual void onRender() override { Layer::onRender(); }
};
} // namespace ce