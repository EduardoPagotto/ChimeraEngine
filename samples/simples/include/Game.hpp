#ifndef __GAME_SIMPLES__HPP
#define __GAME_SIMPLES__HPP

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
    virtual void onRender() override;
    virtual void onUpdate(const uint32_t& count) override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    Chimera::Scene activeScene;
    Chimera::Renderer3d render3D;
    Chimera::Engine* engine;
};

#endif