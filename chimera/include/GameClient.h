#ifndef __GAME_CLIENT_H
#define __GAME_CLIENT_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Singleton.h"
#include "Video.h"
//#include "ChimeraTimer.h"
//#include "ChimeraFont.h"
#include "SceneMng.h"

//#include "ChimeraTransform.h"

//#include "ChimeraHUD.h"
#include "PhysicWorld.h"

namespace Chimera {

class GameClient {
public:
    GameClient ( Video *_pVideo, SceneMng *_pScenMng );
    virtual ~GameClient();

    bool init ( void );
    void gameLoop ( void );
    void done ( void );

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void onFrame() = 0;
    virtual void offFrame() = 0;
    
    virtual void keyCapture ( SDL_Keycode tecla ) = 0;
    virtual void mouseButtonDownCapture ( SDL_MouseButtonEvent mb ) = 0;
    virtual void mouseMotionCapture ( SDL_MouseMotionEvent mm ) = 0;
    
    //virtual void pullEvent(Chimera::Node *_pVal) = 0;

protected:
// 	const SDL_Event *getEventPtr() {
// 		return  &m_eventSDL;
// 	}

    bool m_isActive;
    Video *m_pVideo;
    SceneMng *m_pSceneMng;
    //Font *m_pFont;
    //HUD *m_pHUD;
    PhysicWorld *m_physicWorld;

private:
    
    //SDL_Event m_eventSDL;
    //Timer m_timerFPS;

    bool m_status;
    //float m_fps;
    //std::string m_sFPS;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
