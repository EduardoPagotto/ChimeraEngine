#ifndef __GAME_EVENTS_TEST__H_
# define __GAME_EVENTS_TEST__H_

#include "Video.h"

#include "GameClient.h"
# include "DataMsg.h"


class GameEvents : public Chimera::GameClient
{
public:
    GameEvents();
    virtual ~GameEvents(Chimera::SceneMng *_pScenMng);

    virtual void start();
    virtual void stop();
    virtual void render();
    virtual void keyCapture ( SDL_Keycode tecla );
    virtual void mouseButtonDownCapture ( SDL_MouseButtonEvent mb );
    virtual void mouseButtonUpCapture ( SDL_MouseButtonEvent mb );
    virtual void mouseMotionCapture ( SDL_MouseMotionEvent mm );
    virtual void joystickCapture ( Chimera::JoystickManager &joy );
    virtual void joystickStatus ( Chimera::JoystickManager &joy );
    virtual void userEvent ( const SDL_Event &_event );
    virtual void newFPS ( const unsigned int &fps );
    virtual void beginProcGame();
    virtual void endProcGame();

private :
    GLuint vertexbuffer;
    GLuint programID;
    GLuint VertexArrayID;
};

#endif