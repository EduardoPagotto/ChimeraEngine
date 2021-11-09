#ifndef __GAME_EVENTS_TEST__HPP
#define __GAME_EVENTS_TEST__HPP

#include "chimera/core/Engine.hpp"
#include "chimera/node/Node.hpp"
#include "chimera/node/VisitorRender.hpp"

class Game : public Chimera::IStateMachine {
  public:
    Game(Chimera::Engine* engine, const std::vector<std::string>& shadesFile);
    virtual ~Game();
    // Inherited via IEvents
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    Chimera::Shader shader;
    Chimera::VisitorRender renderV;
    Chimera::CanvasGL* pCanvas;
    Chimera::Node* pRoot;
    Chimera::Engine* engine;
};
#endif