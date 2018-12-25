#ifndef __GAME_TESTE_BSTREE__HPP
#define __GAME_TESTE_BSTREE__HPP

#include "BSPTree.h"
#include "ListPolygon.hpp"
#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/IGameClientEvents.hpp"

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
    void setCube(ListPolygon* _pPolygonList);
    void setOctahedran(ListPolygon* _pPolygonList);
    // ListPolygon* pPolygonList;
    BSPTree* pBspTree;
    int polygon_id;
    GLfloat lightPosition[4]; // = {0, 99.9, 0, 1};
    float eyeX;               // = 0;
    float eyeY;               // = 0;
    float centerX;            // = 0;
    float centerY;            // = 0;

    Chimera::CanvasGL* pCanvas;
    int botaoIndex;
    int estadoBotao;
    bool isPaused;
};

#endif