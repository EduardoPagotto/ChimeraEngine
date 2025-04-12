#pragma once
#include "chimera/render/2d/Layer.hpp"

class TileLayer : public ce::Layer {
  public:
    TileLayer(std::shared_ptr<ce::Shader> shader);
    virtual ~TileLayer();
    virtual void onAttach() override {};
    virtual void onDeatach() override {};
    virtual void onUpdate(ce::IViewProjection& vp, const double& ts) override {};
    virtual bool onEvent(const SDL_Event& event) override;
    virtual void onRender() override;

  private:
    uint16_t x, y;
};
