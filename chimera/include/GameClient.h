#ifndef __GAME_CLIENT_H
#define __GAME_CLIENT_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Singleton.h"
#include "Video.h"
#include "SceneMng.h"
#include "PhysicWorld.h"


namespace Chimera {

class GameClient {
public:
    GameClient (Video *_pVideo, Chimera::SceneMng *_pScenMng);
    virtual ~GameClient();

    void open ();
    void close ();
    void gameLoop ();
    
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void onFrame() = 0;
    virtual void offFrame() = 0;
    virtual void processMsg(DataMsg *_dataMsg) = 0;
    
protected:

    SceneMng *pSceneMng;
    Video *pVideo;
    //Font *m_pFont;
    //HUD *m_pHUD;
    PhysicWorld *physicWorld;
    
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
