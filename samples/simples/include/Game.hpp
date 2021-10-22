#ifndef __GAME_SIMPLES__HPP
#define __GAME_SIMPLES__HPP

#include "chimera/core/Application.hpp"
#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/render/scene/Scene.hpp"

class Game : public Chimera::Application {
  public:
    Game(Chimera::Canvas* canvas);
    virtual ~Game();
    // Inherited via IEvents
    virtual void onStart() override;
    virtual void onUpdate() override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    Chimera::Scene activeScene;
    Chimera::Camera* camera;
    Chimera::Renderer3d render3D;
};

#endif