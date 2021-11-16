#ifndef __GAME_TESTE_TERRAIN__HPP
#define __GAME_TESTE_TERRAIN__HPP

#include "chimera/core/Engine.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "chimera/render/scene/Scene.hpp"

class Game : public Chimera::IStateMachine {
  public:
    Game(Chimera::Engine* engine);
    virtual ~Game();
    // Inherited via IEvents
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual std::string getName() const { return "Game"; }

  private:
    Chimera::Scene activeScene;
    Chimera::Engine* engine;
};

#endif