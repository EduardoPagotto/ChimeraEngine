#ifndef __GAME_CLIENT_H
#define __GAME_CLIENT_H

#include "IGameClientEvents.h"
#include "SceneMng.h"
#include "PhysicsControl.h"
#include "HUD.h"

namespace Chimera
{

class GameClient : public IGameClientEvents
{
public:
    GameClient ( Chimera::SceneMng *_pScenMng );
    virtual ~GameClient();

protected:
    virtual void start();
    virtual void stop();
    virtual void executeColisao ( const Graph::KindOp &_kindOp, Graph::Node *_pNodeA, Graph::Node *_pNodeB ) = 0;
    virtual void newFPS ( const unsigned int &fps );

    void sendMessage ( Graph::KindOp _kindOf, void *_paramA, void *_paramB );

    SceneMng *pSceneMng;
    PhysicsControl *physicWorld;
    Font *pFont;
    HUD *pHUD;

private:
    void userEvent ( const SDL_Event &_event );
    void validaOpColisao ( const SDL_Event &_event );
    void beginProcGame();
    void endProcGame();

    double deadzone;
    std::string textoFPS;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
