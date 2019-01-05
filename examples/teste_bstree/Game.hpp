#ifndef __GAME_TESTE_BSTREE__HPP
#define __GAME_TESTE_BSTREE__HPP

#include "ArrayTriangle.hpp"
#include "BSPTree.h"
#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/IGameClientEvents.hpp"
#include "chimera/core/Logger.hpp"
#include "chimera/core/TrackBall.hpp"

class Game : public Chimera::IGameClientEvents {
  public:
    Game(Chimera::CanvasGL* _pCanvas);

    virtual ~Game();
    // Inherited via IGameClientEvents
    virtual void start();
    virtual void stop();
    virtual void render();
    virtual void keyCapture(SDL_Keycode tecla);
    virtual void mouseButtonDownCapture(SDL_MouseButtonEvent mb);
    virtual void mouseButtonUpCapture(SDL_MouseButtonEvent mb);
    virtual void mouseMotionCapture(SDL_MouseMotionEvent mm);
    virtual void joystickCapture(Chimera::JoystickManager& joy);
    virtual void joystickStatus(Chimera::JoystickManager& joy);
    virtual void newFPS(const unsigned int& fps);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;
    virtual bool paused() override;

  private:
    void setCube(ArrayTriangle* _pArrayTriangle);
    void setOctahedran(ArrayTriangle* _pArrayTriangle);
    void setDrawTest(ArrayTriangle* _pArrayTriangle);
    void setDrawSplit(ArrayTriangle* _pArrayTriangle);

    BSPTree* pBspTree;
    int botaoIndex;
    int estadoBotao;
    glm::vec4 lightPosition;
    Chimera::TrackBall trackBall;

    Chimera::Logger* log;
    int debug_init;

    Chimera::CanvasGL* pCanvas;
    bool isPaused;
};

#endif