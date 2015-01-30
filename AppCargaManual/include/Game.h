#ifndef GAME_XX_H_
#define GAME_XX H_

#include "GameClient.h"
#include "CameraSpherical.h"

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
    virtual void render();
    virtual void keyCapture ( SDL_Keycode tecla );
    virtual void mouseButtonDownCapture ( SDL_MouseButtonEvent mb );
    virtual void mouseButtonUpCapture ( SDL_MouseButtonEvent mb );
    virtual void mouseMotionCapture ( SDL_MouseMotionEvent mm );
    virtual void executeColisao(const Chimera::KindOp &_kindOp, Chimera::Node *_pNodeA, Chimera::Node *_pNodeB);
    virtual void userEvent(const SDL_Event &_event);
    
private:    
    int botaoIndex;
    int estadoBotao;
    
    Chimera::CameraSpherical *pOrbitalCam;
    Chimera::Object *pObj;
    std::string sPosicaoObj;
    //log4cxx::LoggerPtr logger;

};

#endif