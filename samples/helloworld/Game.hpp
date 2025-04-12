#pragma once
#include "TileLayer.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "chimera/render/2d/Label.hpp"

class Game : public ce::IStateMachine {
  public:
    Game(std::shared_ptr<ServiceLocator> sl, ce::Engine* engine);
    virtual ~Game();
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(ce::IViewProjection& vp, const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    std::shared_ptr<ServiceLocator> serviceLoc;
    std::shared_ptr<ce::ICanva> canvas;
    std::shared_ptr<ce::Shader> shader;
    ce::Engine* engine;
    ce::Label* lFPS;
    TileLayer* layer;
    int fps;
};
