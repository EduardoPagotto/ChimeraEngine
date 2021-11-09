#ifndef __HELLOWORD_TILELAYER__HPP
#define __HELLOWORD_TILELAYER__HPP

#include "chimera/render/2d/layer/Layer.hpp"

class TileLayer : public Chimera::Layer {
  public:
    TileLayer(Chimera::Shader* shader);
    virtual ~TileLayer();
    virtual void onAttach() override{};
    virtual void onDeatach() override{};
    virtual void onUpdate() override{};
    virtual bool onEvent(const SDL_Event& event) override;
    virtual std::string getName() const { return "TileLayer"; }
    virtual void onRender() override;

  private:
    uint16_t x, y;
};
#endif