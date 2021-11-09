#ifndef __HELLOWORD_TEST__HPP
#define __HELLOWORD_TEST__HPP

#include "TileLayer.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "chimera/render/2d/Label.hpp"

class Game : public Chimera::IStateMachine {
  public:
    Game(Chimera::Engine* engine);
    virtual ~Game();
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onUpdate() override;
    virtual void onRender() override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    Chimera::Engine* engine;
    Chimera::Shader shader;
    Chimera::Label* lFPS;
    TileLayer* layer;
    int fps;
};

#endif