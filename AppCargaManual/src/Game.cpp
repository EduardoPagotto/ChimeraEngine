#include "Game.h"
#include "ExceptionSDL.h"

#include "Transform.h"
#include "OpenGLDefs.h"

Game::Game ( Chimera::SceneMng *_pScenMng ) : GameClient ( _pScenMng ) {

	pCorpoRigido = nullptr;
	pEmissor = nullptr;
	pOrbitalCam = nullptr;
}

Game::~Game() {
}

void Game::joystickCapture ( Chimera::JoystickManager &joy ) {
}

void Game::joystickStatus ( Chimera::JoystickManager &joy ) {

	using namespace Chimera;
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

		if (joystick->ButtonDown((Uint8)JOY_BUTTON_COD::X) == true) {

			btVector3 posicao = pEmissor->getPosSource();
			posicao.setX(posicao.x() - 0.1f);
			pEmissor->setPosSource( posicao );

		}

		if (joystick->ButtonDown((Uint8)JOY_BUTTON_COD::B) == true) {

			btVector3 posicao = pEmissor->getPosSource();
			posicao.setX(posicao.x() + 0.1);
			pEmissor->setPosSource(posicao);

		}

        int val = joystick->Hat ( 0 );
        if ( val & ( uint8_t ) JOY_PAD_COD::UP ) 
			pCorpoRigido->applyForce ( btVector3 ( 0.0, 0.0, propulsaoLRUD ) );
        
        if ( val & ( uint8_t ) JOY_PAD_COD::DOWN ) 
			pCorpoRigido->applyForce ( btVector3 ( 0.0, 0.0, -propulsaoLRUD ) );
        
        if ( val & ( uint8_t ) JOY_PAD_COD::RIGHT ) 
			pCorpoRigido->applyForce ( btVector3 ( propulsaoLRUD, 0.0, 0.0 ) );
        
        if ( val & ( uint8_t ) JOY_PAD_COD::LEFT ) 
			pCorpoRigido->applyForce ( btVector3 ( -propulsaoLRUD, 0.0, 0.0 ) );
        
        if ( ( roll != 0.0 ) || ( pitch != 0.0 ) || ( yaw != 0.0 ) || ( throttle != 0.0 ) ) {
			pCorpoRigido->applyForce ( btVector3 ( 0.0, throttle, 0.0 ) );
			pCorpoRigido->applyTorc ( btVector3 ( -torque * pitch, -torque * roll, -torque * yaw ) );
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
        sendMessage ( Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN,nullptr, nullptr );
        break;
    case SDLK_UP:
		pCorpoRigido->applyForce ( btVector3 ( 10.0, 0.0, 0.0 ) );
        break;
    case SDLK_DOWN:
		pCorpoRigido->applyForce ( btVector3 ( -10.0, 0.0, 0.0 ) );
        break;
    case SDLK_LEFT:
		pCorpoRigido->applyForce ( btVector3 ( 0.0, 10.0, 0.0 ) );
        break;
    case SDLK_RIGHT:
		pCorpoRigido->applyForce ( btVector3 ( 0.0, -10.0, 0.0 ) );
        break;
    case SDLK_a:
		pCorpoRigido->applyTorc ( btVector3 ( 0.0, 0.0, 10.0 ) );
        break;
    case SDLK_s:
		pCorpoRigido->applyTorc ( btVector3 ( 0.0, 0.0, -10.0 ) );
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
	Chimera::Transform* pSkyBox = ( Chimera::Transform* ) Chimera::Node::findNodeByName( Chimera::EntityKind::TRANSFORM, "SkyBox" );
	Chimera::Draw *pDraw = (Chimera::Draw*)pSkyBox->findChildByKind(Chimera::EntityKind::DRAW, 0);

	pDraw->getState()->setEnableLight(Chimera::LightNum::LIGHTING, false);
	pDraw->getState()->setEnableColorMaterial(Chimera::ColorMaterial::COLOR_MATERIAL, true);

    //Pega primeira camera
    pOrbitalCam = ( Chimera::CameraSpherical* ) pSceneMng->getNode ( Chimera::EntityKind::CAMERA, 0 );

    //Ajusta objeto como o primario
	pCorpoRigido = ( Chimera::Solid* )Chimera::Node::findNodeByName( Chimera::EntityKind::PHYSICS, "Zoltan" );

	Chimera::Light *pLight = (Chimera::Light*) pSceneMng->getNode(Chimera::EntityKind::LIGHT, 0);

	pEmissor = (Chimera::ParticleEmitter*) Chimera::Node::findNodeByName(Chimera::EntityKind::DRAW, "testeZ1");

    pSceneMng->cameraAtiva ( pOrbitalCam );
    pSceneMng->objetoAtivo (pCorpoRigido);

	//ajusta scene root com luz e material ativo
	pSceneMng->getRoot()->getState()->setEnableLight(Chimera::LightNum::LIGHTING, true);
	pSceneMng->getRoot()->getState()->setEnableLight(Chimera::LightNum::LIGHT0, true);
	pSceneMng->getRoot()->getState()->setEnableLighting(pLight, true);
	pSceneMng->getRoot()->getState()->setEnableColorMaterial(Chimera::ColorMaterial::COLOR_MATERIAL, false);

    sPosicaoObj = "pos:(,,)";

    pHUD->addText ( 0, 0, 255, 0, Chimera::Color::BLUE, &sPosicaoObj );
}

void Game::stop() {
    GameClient::stop();
}

void Game::newFPS ( const unsigned int &fps ) {

    btVector3 val1 = pCorpoRigido->getPosition();
    sPosicaoObj = "pos:(" + std::to_string ( val1.getX() ) + "," + std::to_string ( val1.getY() ) + "," + std::to_string ( val1.getZ() ) + ")";

    GameClient::newFPS ( fps );
}

void Game::render() {

    pSceneMng->draw ( pHUD );

}

void Game::executeColisao ( const Chimera::KindOp &_kindOp, Chimera::Node *_pNodeA, Chimera::Node *_pNodeB ) {

    std::string l_msg;

    switch ( _kindOp ) {
    case Chimera::KindOp::START_COLLIDE:
        l_msg = " START ";
        break;
    case Chimera::KindOp::ON_COLLIDE:
        l_msg = " ON ";
        break;
    case Chimera::KindOp::OFF_COLLIDE:
        l_msg = " OFF ";
        break;
    default:
        l_msg = " DESCONHECIDO ";
        break;
    }

    std::string l_completa = "Colisao cod:" + l_msg + "ObjA:" + _pNodeA->getName() + " ObjB:" + _pNodeB->getName();
}
