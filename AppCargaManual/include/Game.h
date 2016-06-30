#ifndef GAME_XX_H_
#define GAME_XX H_

#include "GameClient.h"
#include "CameraSpherical.h"

/**
 * Class Game
 *  @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class Game : public Chimera::GameClient
{
public:
    Game ( Chimera::SceneMng *_pScenMng );
    virtual ~Game();

    virtual void start();
    virtual void stop();
    virtual void render();
    virtual void keyCapture ( SDL_Keycode tecla );
    virtual void mouseButtonDownCapture ( SDL_MouseButtonEvent mb );
    virtual void mouseButtonUpCapture ( SDL_MouseButtonEvent mb );
    virtual void mouseMotionCapture ( SDL_MouseMotionEvent mm );
    virtual void joystickCapture ( Chimera::Device::JoystickManager &joy );
    virtual void joystickStatus ( Chimera::Device::JoystickManager &joy );
    virtual void executeColisao ( const Chimera::Graph::KindOp &_kindOp, Chimera::Graph::Node *_pNodeA, Chimera::Graph::Node *_pNodeB );
    virtual void newFPS ( const unsigned int &fps );

private:
    int botaoIndex;
    int estadoBotao;

    Chimera::Graph::CameraSpherical *pOrbitalCam;
    Chimera::Graph::Object *pObj;
    std::string sPosicaoObj;

    Chimera::Graph::SkyBox* pSkyBox;
};

#endif