#ifndef __GAME_EVENTS_TEST__HPP
#define __GAME_EVENTS_TEST__HPP

#include "chimera/core/io/IEvents.hpp"
#include "chimera/core/windows/CanvasGL.hpp"
#include "chimera/node/Node.hpp"
#include "chimera/node/VisitorRender.hpp"

class Game : public Chimera::Core::IEvents {
  public:
    Game(Chimera::Core::CanvasGL* _pCanvas, Chimera::Node* _pRoot);
    virtual ~Game();
    // Inherited via IEvents
    virtual void onStart() override;
    virtual void onUpdate() override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    Chimera::VisitorRender renderV;
    Chimera::Core::CanvasGL* pCanvas;
    Chimera::Node* pRoot;
};

#endif