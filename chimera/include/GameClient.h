#ifndef __GAME_CLIENT_H
#define __GAME_CLIENT_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Singleton.h"
#include "Video.h"
//#include "ChimeraTimer.h"
//#include "ChimeraFont.h"
#include "Engine3D.h"
#include "SceneMng.h"


//#include "ChimeraTransform.h"

//#include "ChimeraHUD.h"
#include "PhysicWorld.h"


namespace Chimera {

class GameClient {
public:
    GameClient ( Engine3D *pEngine3D, SceneMng *_pScenMng );
    virtual ~GameClient();

    void open ();
    void close ();
    void gameLoop ();
    
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void onFrame() = 0;
    virtual void offFrame() = 0;
    virtual void processMsg(DataMsg *dataMsg) = 0;
    
protected:

    Engine3D *m_pEngined3D;
    SceneMng *m_pSceneMng;
    //Font *m_pFont;
    //HUD *m_pHUD;
    PhysicWorld *m_physicWorld;
    
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
