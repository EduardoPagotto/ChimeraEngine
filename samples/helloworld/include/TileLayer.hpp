#pragma once
#include "chimera_render/2d/Layer.hpp"

class TileLayer : public ce::Layer {
  public:
    TileLayer(std::shared_ptr<ce::Shader> shader);
    virtual ~TileLayer() = default;
    virtual void on_attach() override {};
    virtual void on_deatach() override {};
    virtual void on_update(const double& ts) override {};
    virtual void on_event(const SDL_Event& event) override;
    virtual void on_render() override;
    std::string get_name() const override { return "GAME"; }

  private:
    uint16_t x_, y_;
};
