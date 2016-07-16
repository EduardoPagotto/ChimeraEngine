#include "GameClient.h"
#include "Singleton.h"
#include "ExceptionSDL.h"

#include <iostream>

namespace Chimera {

GameClient::GameClient ( Chimera::SceneMng *_pScenMng ) : pSceneMng ( _pScenMng )  {

    physicWorld = Infra::Singleton<PhysicsControl>::getRefSingleton();
}

GameClient::~GameClient() {

    Infra::Singleton<PhysicsControl>::releaseRefSingleton();
}

void GameClient::newFPS ( const unsigned int &fps ) {
   // textoFPS = "fps: " + std::to_string ( fps ) + std::string ( " Periodo: " ) + std::to_string ( physicWorld->getLastPeriod() );
}

void GameClient::start() {

    deadzone = 0.02;
    pSceneMng->init();

}

void GameClient::stop() {

}

void GameClient::beginProcGame() {

    physicWorld->stepSim();
    physicWorld->checkCollisions();

}

void GameClient::endProcGame() {

}

void GameClient::userEvent ( const SDL_Event &_event ) {

    KindOp op = ( KindOp ) _event.user.code;
    if ( ( op == Chimera::KindOp::START_COLLIDE ) ||
            ( op == Chimera::KindOp::ON_COLLIDE ) ||
            ( op == Chimera::KindOp::OFF_COLLIDE ) ) {

        executeColisao ( op, ( Node* ) _event.user.data1, ( Node* ) _event.user.data2 );
    }
}

void GameClient::sendMessage ( KindOp _kindOf, void *_paramA, void *_paramB ) {

    SDL_Event event;
    SDL_zero ( event );
    event.type = SDL_USEREVENT;
    event.user.code = ( int ) _kindOf;
    event.user.data1 = _paramA;
    event.user.data2 = _paramB;
    SDL_PushEvent ( &event );

}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
