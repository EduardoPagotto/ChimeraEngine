#pragma once
#include "Layer.hpp"

class Tile : public Chimera::Layer {
  public:
    Tile(const std::string& name, Chimera::IRenderer2D* renderer, Chimera::Shader shader, Chimera::Camera* camera);
    virtual ~Tile();
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onUpdate(Chimera::ViewProjection& vp, const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual void onRender() override;
    virtual std::string getName() const override { return nameTile; }

  private:
    std::string nameTile;
};