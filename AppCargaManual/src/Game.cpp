#include "Game.h"
#include "ExceptionSDL.h"

Game::Game ( Chimera::SceneMng *_pScenMng ) : GameClient ( _pScenMng ) {
}

Game::~Game() {
}

void Game::joystickCapture ( Chimera::Device::JoystickManager &joy ) {
}

void Game::joystickStatus ( Chimera::Device::JoystickManager &joy ) {

    using namespace Chimera::Device;
    // Captura joystick 0 se existir
    JoystickState *joystick = joy.getJoystickState ( 0 );
    if ( joystick != nullptr ) {

        float propulsaoLRUD = 1.0f;
        float propulsaoPrincipal = 3.0f;
        float propulsaoFrontal = 1.0f;
        float torque = 0.1f;
        float deadZone = 0.5f;
        
        float yaw = joystick->Axis ( ( Uint8 ) JOY_AXIX_COD::LEFT_X, deadZone );
        float pitch = joystick->Axis ( ( Uint8 ) JOY_AXIX_COD::LEFT_Y, deadZone );
        float roll = joystick->Axis ( ( Uint8 ) JOY_AXIX_COD::RIGHT_X, deadZone );

        double throttle = -propulsaoPrincipal * ( ( 1 + joystick->Axis ( ( Uint8 ) JOY_AXIX_COD::LEFT_TRIGGER, deadZone ) ) / 2 );
        throttle = throttle - ( -propulsaoFrontal* ( ( 1 + joystick->Axis ( ( Uint8 ) JOY_AXIX_COD::RIGHT_TRIGGER, deadZone ) ) / 2 ) );

        int val = joystick->Hat ( 0 );
        if ( val & ( uint8_t ) JOY_PAD_COD::UP ) 
            pObj->applyForce ( btVector3 ( 0.0, 0.0, propulsaoLRUD ) );
        
        if ( val & ( uint8_t ) JOY_PAD_COD::DOWN ) 
            pObj->applyForce ( btVector3 ( 0.0, 0.0, -propulsaoLRUD ) );
        
        if ( val & ( uint8_t ) JOY_PAD_COD::RIGHT ) 
            pObj->applyForce ( btVector3 ( propulsaoLRUD, 0.0, 0.0 ) );
        
        if ( val & ( uint8_t ) JOY_PAD_COD::LEFT ) 
            pObj->applyForce ( btVector3 ( -propulsaoLRUD, 0.0, 0.0 ) );
        
        if ( ( roll != 0.0 ) || ( pitch != 0.0 ) || ( yaw != 0.0 ) || ( throttle != 0.0 ) ) {
            pObj->applyForce ( btVector3 ( 0.0, throttle, 0.0 ) );
            pObj->applyTorc ( btVector3 ( -torque * pitch, -torque * roll, -torque * yaw ) );
        }
    }
}

void Game::keyCapture ( SDL_Keycode tecla ) {

    switch ( tecla ) {
    case SDLK_ESCAPE:
        SDL_Event l_eventQuit;
        l_eventQuit.type = SDL_QUIT;
        if ( SDL_PushEvent ( &l_eventQuit ) == -1 ) {
            throw Chimera::ExceptionSDL ( Chimera::ExceptionCode::CLOSE, std::string ( SDL_GetError() ) );
        }
        break;
    case SDLK_F1:
        pHUD->setOn ( !pHUD->isOn() );
        break;
    case SDLK_F10:
        sendMessage ( Chimera::Graph::KindOp::VIDEO_TOGGLE_FULL_SCREEN,nullptr, nullptr );
        break;
    case SDLK_UP:
        pObj->applyForce ( btVector3 ( 10.0, 0.0, 0.0 ) );
        break;
    case SDLK_DOWN:
        pObj->applyForce ( btVector3 ( -10.0, 0.0, 0.0 ) );
        break;
    case SDLK_LEFT:
        pObj->applyForce ( btVector3 ( 0.0, 10.0, 0.0 ) );
        break;
    case SDLK_RIGHT:
        pObj->applyForce ( btVector3 ( 0.0, -10.0, 0.0 ) );
        break;
    case SDLK_a:
        pObj->applyTorc ( btVector3 ( 0.0, 0.0, 10.0 ) );
        break;
    case SDLK_s:
        pObj->applyTorc ( btVector3 ( 0.0, 0.0, -10.0 ) );
        break;
    default:
        break;
    }
}

void Game::mouseButtonUpCapture ( SDL_MouseButtonEvent mb ) {
    botaoIndex = mb.button;
    estadoBotao = mb.state;
}

void Game::mouseButtonDownCapture ( SDL_MouseButtonEvent mb ) {
    botaoIndex = mb.button;
    estadoBotao = mb.state;
}

void Game::mouseMotionCapture ( SDL_MouseMotionEvent mm ) {

    if ( estadoBotao == SDL_PRESSED ) {
        if ( botaoIndex == 1 ) {
            pOrbitalCam->trackBall ( mm.yrel, mm.xrel, 0 );
        } else if ( botaoIndex == 2 ) {
            pOrbitalCam->trackBall ( 0, 0, mm.yrel );
        }
    }
}

void Game::start() {

    GameClient::start();

    // Pega o Skybox
    pSkyBox = ( Chimera::Graph::SkyBox* ) pSceneMng->getNode ( Chimera::EntityKind::SKYBOX, 0 );

    //Pega primeira camera
    pOrbitalCam = ( Chimera::Graph::CameraSpherical* ) pSceneMng->getNode ( Chimera::EntityKind::CAMERA, 0 );

    //Ajusta objeto como o primario
    pObj = ( Chimera::Graph::Object* ) pSceneMng->getNode ( Chimera::EntityKind::OBJECT, "Zoltan" );

    pSceneMng->skyBoxAtivo ( pSkyBox );
    pSceneMng->cameraAtiva ( pOrbitalCam );
    pSceneMng->objetoAtivo ( pObj );
    pSceneMng->setLight ( true );
    pSceneMng->setMaterial ( true );

    sPosicaoObj = "pos:(,,)";

    pHUD->addText ( 0, 0, 255, 0, Chimera::Color::BLUE, &sPosicaoObj );
}

void Game::stop() {
    GameClient::stop();
}

void Game::newFPS ( const unsigned int &fps ) {

    btVector3 val1 = pObj->getPosition();
    sPosicaoObj = "pos:(" + std::to_string ( val1.getX() ) + "," + std::to_string ( val1.getY() ) + "," + std::to_string ( val1.getZ() ) + ")";

    GameClient::newFPS ( fps );
}

void Game::render() {

    pSceneMng->draw ( pHUD );

}

void Game::executeColisao ( const Chimera::Graph::KindOp &_kindOp, Chimera::Graph::Node *_pNodeA, Chimera::Graph::Node *_pNodeB ) {

    std::string l_msg;

    switch ( _kindOp ) {
    case Chimera::Graph::KindOp::START_COLLIDE:
        l_msg = " START ";
        break;
    case Chimera::Graph::KindOp::ON_COLLIDE:
        l_msg = " ON ";
        break;
    case Chimera::Graph::KindOp::OFF_COLLIDE:
        l_msg = " OFF ";
        break;
    default:
        l_msg = " DESCONHECIDO ";
        break;
    }

    std::string l_completa = "Colisao cod:" + l_msg + "ObjA:" + _pNodeA->getName() + " ObjB:" + _pNodeB->getName();

}
