// #ifndef __GAME_CLIENT_H
// #define __GAME_CLIENT_H
// 
// #include "IGameClientEvents.h"
// #include "SceneMng.h"
// #include "PhysicsControl.h"
// 
// namespace Chimera
// {
// 
// class GameClient : public IGameClientEvents
// {
// public:
//     GameClient ( Chimera::SceneMng *_pScenMng );
//     virtual ~GameClient();
// 
// protected:
//     virtual void start();
//     virtual void stop();
//     virtual void executeColisao ( const KindOp &_kindOp, Node *_pNodeA, Node *_pNodeB ) = 0;
//     virtual void newFPS ( const unsigned int &fps );
// 
//     void sendMessage ( KindOp _kindOf, void *_paramA, void *_paramB );
// 
// 
//    
// private:
//     void userEvent ( const SDL_Event &_event );
//     //void validaColisao ( const SDL_Event &_event );
//     void beginProcGame();
//     void endProcGame();
// 
//     //double deadzone;
// };
// }
// 
// #endif
// // kate: indent-mode cstyle; indent-width 4; replace-tabs on;
