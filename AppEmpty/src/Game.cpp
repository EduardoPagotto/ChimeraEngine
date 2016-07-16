#include "Game.h"
#include "ExceptionSDL.h"

#include "Transform.h"
#include "OpenGLDefs.h"

Game::Game(Chimera::SceneMng *_pScenMng) : GameClient(_pScenMng) {
}

Game::~Game() {
}

void Game::joystickCapture(Chimera::JoystickManager &joy) {
}

void Game::joystickStatus(Chimera::JoystickManager &joy) {

	using namespace Chimera;
	// Captura joystick 0 se existir
	JoystickState *joystick = joy.getJoystickState(0);
	if (joystick != nullptr) {

		float deadZone = 0.5f;
		float propulsaoPrincipal = 3.0f;
		float propulsaoFrontal = 1.0f;

		float yaw = joystick->Axis((Uint8)JOY_AXIX_COD::LEFT_X, deadZone);
		float pitch = joystick->Axis((Uint8)JOY_AXIX_COD::LEFT_Y, deadZone);
		float roll = joystick->Axis((Uint8)JOY_AXIX_COD::RIGHT_X, deadZone);

		double throttle = -propulsaoPrincipal * ((1 + joystick->Axis((Uint8)JOY_AXIX_COD::LEFT_TRIGGER, deadZone)) / 2);
		throttle = throttle - (-propulsaoFrontal* ((1 + joystick->Axis((Uint8)JOY_AXIX_COD::RIGHT_TRIGGER, deadZone)) / 2));

		if (joystick->ButtonDown((Uint8)JOY_BUTTON_COD::X) == true) {}
		if (joystick->ButtonDown((Uint8)JOY_BUTTON_COD::B) == true) {}

		int val = joystick->Hat(0);
		if (val & (uint8_t)JOY_PAD_COD::UP) {}
		if (val & (uint8_t)JOY_PAD_COD::DOWN) {}
		if (val & (uint8_t)JOY_PAD_COD::RIGHT) {}
		if (val & (uint8_t)JOY_PAD_COD::LEFT) {}
		if ((roll != 0.0) || (pitch != 0.0) || (yaw != 0.0) || (throttle != 0.0)) {}
	}
}

void Game::keyCapture(SDL_Keycode tecla) {

	switch (tecla) {
	case SDLK_ESCAPE:
		SDL_Event l_eventQuit;
		l_eventQuit.type = SDL_QUIT;
		if (SDL_PushEvent(&l_eventQuit) == -1) {
			throw Chimera::ExceptionSDL(Chimera::ExceptionCode::CLOSE, std::string(SDL_GetError()));
		}
		break;
	case SDLK_F1:
		//pHUD->setOn(!pHUD->isOn());
		break;
	case SDLK_F10:
		sendMessage(Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN, nullptr, nullptr);
		break;
	case SDLK_UP:
		break;
	case SDLK_DOWN:
		break;
	case SDLK_LEFT:
		break;
	case SDLK_RIGHT:
		break;
	case SDLK_a:
		break;
	case SDLK_s:
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
			//pOrbitalCam->trackBall(mm.yrel, mm.xrel, 0);
		} else if (botaoIndex == 2) {
			//pOrbitalCam->trackBall(0, 0, mm.yrel);
		}
	}
}

void Game::start() {

	GameClient::start();

	Chimera::Transform *pOrigem = new Chimera::Transform(nullptr, "origem");

	pSceneMng->cameraAtiva(nullptr);
	pSceneMng->objetoAtivo(pOrigem);
}

void Game::stop() {
	GameClient::stop();
}

void Game::newFPS(const unsigned int &fps) {

	GameClient::newFPS(fps);
}

void Game::render() {

	pSceneMng->draw();
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
		l_msg = " DESCONHECIDO ";
		break;
	}

	std::string l_completa = "Colisao cod:" + l_msg + "ObjA:" + _pNodeA->getName() + " ObjB:" + _pNodeB->getName();
}
