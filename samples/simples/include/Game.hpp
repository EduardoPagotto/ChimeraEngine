#pragma once
#include "chimera/render/scene/Scene.hpp"

class Game : public Chimera::IStateMachine {
  public:
    Game(Chimera::Scene& scene);
    virtual ~Game();
    // Inherited via IEvents
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(Chimera::ViewProjection& vp, const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    Chimera::Scene* scene;
};
