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
    
    //virtual void update ( Chimera::DataMsg *dataMsg );

    void keyCapture ( SDL_Keycode tecla );
    void mouseButtonDownCapture ( SDL_MouseButtonEvent mb );
    void mouseMotionCapture ( SDL_MouseMotionEvent mm );

};

#endif /* GAME_H_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
