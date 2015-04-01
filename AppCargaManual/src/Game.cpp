#include "Game.h"

Game::Game(Chimera::Video *_pVideo, Chimera::SceneMng *_pScenMng) : GameClient(_pVideo, _pScenMng) {

}

Game::~Game() {
}

void Game::joystickCapture(Chimera::JoystickManager &joy) {
	//float axis = joy.Axis(0, 0, 0, 0);

	//if (axis != 0.0) {
	//	pObj->applyForce(btVector3(10.0 * axis, 0.0, 0.0));
	//	printf("axis: %f\n", 10.0 * axis);
	//}

}


void Game::getStatusJoystick() {


	// Captura joystick 0 se existir
	Chimera::JoystickState *joystick = joystickManager.getJoystickState(0);
	if (joystick !=  nullptr) {

	 float propulsaoLRUD = 1.0;
	 float propulsaoPrincipal = 3.0;
	 float propulsaoFrontal = 1.0;
	 float torque = 0.1;

	 float roll = joystick->Axis(0, 0.2);
	 float pitch = joystick->Axis( 1, 0.2);
	 float yaw = joystick->Axis( 2, 0.2);

	 double throttle = -propulsaoPrincipal * ((1 + joystick->Axis(4, 0.1)) / 2);
	 throttle = throttle - (-propulsaoFrontal* ((1 + joystick->Axis( 5, 0.1)) / 2));

	 bool propUp = joystick->ButtonDown(0);
	 if (propUp == true) {
	  pObj->applyForce(btVector3(0.0, 0.0, propulsaoLRUD));
	}

	 bool propDown = joystick->ButtonDown(1);
	 if (propDown == true) {
	  pObj->applyForce(btVector3(0.0, 0.0, -propulsaoLRUD));
	}

	 bool propLeft = joystick->ButtonDown(2);
	 if (propLeft == true) {
	  pObj->applyForce(btVector3(propulsaoLRUD, 0.0, 0.0));
	}

	 bool propRight = joystick->ButtonDown( 3);
	 if (propRight == true) {
	  pObj->applyForce(btVector3(-propulsaoLRUD, 0.0, 0.0));
	}

	 if ((roll != 0.0) || (pitch != 0.0) || (yaw != 0.0) || (throttle != 0.0)) {
	  pObj->applyForce(btVector3(0.0, throttle, 0.0));
	  pObj->applyTorc(btVector3(-torque * pitch, -torque * roll, -torque * yaw));
	}


	}

}

void Game::keyCapture(SDL_Keycode tecla) {

	switch (tecla) {
	case SDLK_ESCAPE:
		GameClient::close();
		break;
	case SDLK_F1:
		pHUD->setOn(!pHUD->isOn());
		break;
	case SDLK_F10:
		pVideo->toggleFullScreen();
		break;
	case SDLK_UP:
		pObj->applyForce(btVector3(10.0, 0.0, 0.0));
		break;
	case SDLK_DOWN:
		pObj->applyForce(btVector3(-10.0, 0.0, 0.0));
		break;
	case SDLK_LEFT:
		pObj->applyForce(btVector3(0.0, 10.0, 0.0));
		break;
	case SDLK_RIGHT:
		pObj->applyForce(btVector3(0.0, -10.0, 0.0));
		break;
	case SDLK_MINUS:
		pObj->applyTorc(btVector3(0.0, -10.0, 0.0));
		break;
	case SDLK_PLUS:
		pObj->applyTorc(btVector3(0.0, 10.0, 0.0));
		break;
	default:
		break;
	}

}

void Game::mouseButtonUpCapture(SDL_MouseButtonEvent mb) {

	botaoIndex = mb.button;
	estadoBotao = mb.state;

}

void Game::mouseButtonDownCapture(SDL_MouseButtonEvent mb) {

	botaoIndex = mb.button;
	estadoBotao = mb.state;

}

void Game::mouseMotionCapture(SDL_MouseMotionEvent mm) {

	if (estadoBotao == SDL_PRESSED) {
		if (botaoIndex == 1) {
			pOrbitalCam->trackBall(mm.yrel, mm.xrel, 0);
		}
		else if (botaoIndex == 2) {
			pOrbitalCam->trackBall(0, 0, mm.yrel);
		}
	}

}

void Game::start() {

	using namespace Chimera;

	//instancia e coloca na cena uma nova camera orbital
	pOrbitalCam = (CameraSpherical*)pSceneMng->getNode(EntityKind::CAMERA, 0);

	//Ajusta objeto primario
	pObj = (Object*)pSceneMng->getNode(EntityKind::OBJECT,"Zoltan" );

	// FIXME: Quando modificar o dae remover esta entrada
	//pObj->addChild(pOrbitalCam);

	pSkyBox = (SkyBox*)pSceneMng->getNode(EntityKind::SKYBOX, 0);
	pSceneMng->skyBoxAtivo(pSkyBox);

	pVideo->setLight(true);
	pVideo->setMaterial(true);

	sPosicaoObj = "pos:(,,)";

	pHUD->addText(0, 0, 255, 0, Color::BLUE, &sPosicaoObj);

}

void Game::stop(){
}

void Game::render() {

	using namespace Chimera;

	pSceneMng->cameraAtiva(pOrbitalCam);
	pSceneMng->objetoAtivo(pObj);

	btVector3 val1 = pObj->getPosition();
	sPosicaoObj = "pos:(" + std::to_string(val1.getX()) + "," + std::to_string(val1.getY()) + "," + std::to_string(val1.getZ()) + ")";

	int indiceDesenho = 1;
	if (pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS)
		indiceDesenho = 2;



	for (int eye = 0; eye < indiceDesenho; eye++) {


		pSceneMng->RenderSceneA();

		pVideo->executeViewPerspective(pOrbitalCam, eye);

		pOrbitalCam->exec();

		if (pSkyBox != nullptr)
			pSkyBox->render();

// 	 		Chimera::DataMsg dataMsg(KindOp::DRAW3D, this, pObjeto, nullptr);
// 	 		update(&dataMsg);
//
// 	         for (Light *pLight : m_vLight) {
// 	             pLight->exec();
// 	         }

	 		//execLight();

		pSceneMng->ApplyShadowMap();


		if (pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS)
			pVideo->updateHud(pHUD, 0);
		else
			pVideo->updateHud(pHUD, eye);
	}

	getStatusJoystick();

}

void Game::executeColisao(const Chimera::KindOp &_kindOp, Chimera::Node *_pNodeA, Chimera::Node *_pNodeB) {

	std::string l_msg;

	switch (_kindOp) {
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
		//userEvent(pEvento);
		break;
	}

	std::string l_completa = "Colisao cod:" + l_msg + "ObjA:" + _pNodeA->getId() + " ObjB:" + _pNodeB->getId();

}

void Game::userEvent(const SDL_Event &_event) {
	//LOG4CXX_INFO ( logger ,"Evento nao implemtentado" );
}
