#ifndef __GAME_EVENTS_TEST__H_
# define __GAME_EVENTS_TEST__H_

#include <IGameClientEvents.h>
#include <DataMsg.h>
#include <Node.h>
#include <SceneMng.h>

class Game : public Chimera::IGameClientEvents 
{
public:
    Game ( Chimera::SceneMng *_pScenMng );
    virtual ~Game();
    // Inherited via IGameClientEvents
    virtual void start();
    virtual void stop();
    virtual void render();
    virtual void keyCapture(SDL_Keycode tecla);
    virtual void mouseButtonDownCapture(SDL_MouseButtonEvent mb);
    virtual void mouseButtonUpCapture(SDL_MouseButtonEvent mb);
    virtual void mouseMotionCapture(SDL_MouseMotionEvent mm);
    virtual void joystickCapture(Chimera::JoystickManager &joy);
    virtual void joystickStatus(Chimera::JoystickManager &joy);
    virtual void newFPS(const unsigned int &fps);
    virtual void userEvent(const SDL_Event & _event) override;
    virtual void windowEvent(const SDL_WindowEvent & _event) override;
    virtual bool paused() override;

private:
    
    Chimera::SceneMng *pSceneMng;
    
    int botaoIndex;
    int estadoBotao;
    bool isPaused;
};

#endif