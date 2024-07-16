#pragma once
#include "TileLayer.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "chimera/render/2d/Label.hpp"

class Game : public Chimera::IStateMachine {
  public:
    Game(std::shared_ptr<ServiceLocator> sl);
    virtual ~Game();
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(Chimera::IViewProjection& vp, const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    std::shared_ptr<Chimera::ICanva> canvas;
    Chimera::Shader shader;
    Chimera::Label* lFPS;
    TileLayer* layer;
    int fps;
};
