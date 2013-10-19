#ifndef GAME_H_
#define GAME_H_

#include "GameClient.h"

/**
 * Class Game
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Game: public Chimera::GameClient {
public:
    Game(Chimera::Video *_pVideo, Chimera::SceneMng *_pScenMng);
    virtual ~Game();

    virtual void start();
    virtual void stop();
    virtual void onFrame();
    virtual void offFrame();
    virtual void processMsg(Chimera::DataMsg *dataMsg);
    
    bool keyCapture ( SDL_Keycode tecla );
    bool mouseButtonDownCapture ( SDL_MouseButtonEvent mb );
    bool mouseMotionCapture ( SDL_MouseMotionEvent mm );
private:
    
    Chimera::Timer timer01;
    Chimera::Camera *pCam;
    Chimera::Object *pObj;

    log4cxx::LoggerPtr logger;
    
    float xrot;
    float yrot;
};

#endif /* GAME_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
