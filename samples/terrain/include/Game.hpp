#ifndef __GAME_TESTE_TERRAIN__HPP
#define __GAME_TESTE_TERRAIN__HPP

#include "chimera/core/Engine.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/render/scene/Scene.hpp"

class Game : public Chimera::IStateMachine {
  public:
    Game(Chimera::Engine* engine) : engine(engine){};
    virtual ~Game();
    // Inherited via IEvents
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onUpdate() override;
    virtual void onRender() override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    Chimera::Scene activeScene;
    Chimera::Renderer3d render3d;
    Chimera::Engine* engine;
};

#endif