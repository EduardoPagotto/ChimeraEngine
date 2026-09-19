#pragma once
#include "chimera_render/2d/Layer.hpp"

class TileLayer : public ce::Layer {

  private:
    uint16_t x, y;

  public:
    TileLayer(std::shared_ptr<ce::Shader> shader);
    virtual ~TileLayer();
    virtual void onAttach() override {};
    virtual void onDeatach() override {};
    virtual void onUpdate(const double& ts) override {};
    virtual void onEvent(const SDL_Event& event) override;
    virtual void onRender() override;
    std::string getName() const override { return "GAME"; }
};
