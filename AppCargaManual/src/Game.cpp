#include "Game.h"

Game::Game(Chimera::Video *_pVideo, Chimera::SceneMng *_pScenMng) : GameClient(_pVideo, _pScenMng) {

	//logger = log4cxx::Logger::getLogger ( "Game" );
}

Game::~Game() {
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

	physicWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));

	//instancia e coloca na cena uma nova camera orbital
	Camera* pCam = (Camera*)pSceneMng->getNode(EntityKind::CAMERA, 0);
	pOrbitalCam = new CameraSpherical(*pCam);
	pOrbitalCam->setDistanciaMaxima(1000.0f);
	pOrbitalCam->setDistanciaMinima(0.5f);
	pOrbitalCam->setId("Orbital1");
	pSceneMng->addChildToScene(pOrbitalCam);

	//Ajusta objeto primario
	//pObj = (Object*)pSceneMng->getNode(EntityKind::OBJECT, 0);//Cube.001
	//pObj = (Object*)pSceneMng->getRoot()->findNodeById(EntityKind::OBJECT, "Zoltan");//Zoltan//cubo01
	pObj = (Object*)pSceneMng->getRoot()->findNodeById(EntityKind::OBJECT, "cubo01");//Zoltan//cubo01

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

		pVideo->executeViewPerspective(pOrbitalCam, eye);

		pSceneMng->draw3d();

		if (pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS)
			pVideo->updateHud(pHUD, 0);
		else
			pVideo->updateHud(pHUD, eye);
	}



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
	//LOG4CXX_INFO ( logger ,l_completa );

}

void Game::userEvent(const SDL_Event &_event) {
	//LOG4CXX_INFO ( logger ,"Evento nao implemtentado" );
}
