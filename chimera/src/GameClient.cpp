#include "GameClient.h"
#include "Singleton.h"
#include "ExceptionSDL.h"

#include <iostream>

namespace Chimera {

GameClient::GameClient ( Chimera::SceneMng *_pScenMng ) : pSceneMng ( _pScenMng )  {

    textoFPS = "fps: 0";
    pHUD = new HUD();

#ifdef WIN32
    pFont = new Chimera::Font ( "C:\\Projetos\\ChimeraEngine\\fonts\\FreeSans.ttf", 18 );
#else
    pFont = new Chimera::Font ( "../../fonts/FreeSans.ttf", 18 );
#endif

    pHUD->addFont ( pFont );

    SDL_Rect area = { 30, 30, 600, 20 };

    pHUD->addSquare ( area, Color ( 1.0f, 1.0f, 1.0f, 0.2f ) );
    pHUD->addText ( 0, 0, 0, 0, Color::RED, &textoFPS );

    physicWorld = Infra::Singleton<PhysicsControl>::getRefSingleton();
}

GameClient::~GameClient() {

    if ( pHUD ) {
        delete pHUD;
    }

    Infra::Singleton<PhysicsControl>::releaseRefSingleton();
}

void GameClient::newFPS ( const unsigned int &fps ) {
    textoFPS = "fps: " + std::to_string ( fps ) + std::string ( " Periodo: " ) + std::to_string ( physicWorld->getLastPeriod() );
}

void GameClient::start() {

    deadzone = 0.02;

    Graph::DataMsg dataMsg ( Graph::KindOp::START, this, nullptr, nullptr );
    pSceneMng->update ( &dataMsg );

    pHUD->setOn ( true );
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

    Graph::KindOp op = ( Graph::KindOp ) _event.user.code;
    if ( ( op == Chimera::Graph::KindOp::START_COLLIDE ) ||
            ( op == Chimera::Graph::KindOp::ON_COLLIDE ) ||
            ( op == Chimera::Graph::KindOp::OFF_COLLIDE ) ) {

        executeColisao ( op, ( Graph::Node* ) _event.user.data1, ( Graph::Node* ) _event.user.data2 );
    }
}

void GameClient::sendMessage ( Graph::KindOp _kindOf, void *_paramA, void *_paramB ) {

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
