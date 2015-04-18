#include "GameClient.h"
#include "Singleton.h"
#include "ExceptionSDL.h"

#include <iostream>

namespace Chimera {

	GameClient::GameClient(Video *_pVideo, Chimera::SceneMng *_pScenMng) : FlowControl(_pVideo), pSceneMng(_pScenMng)  {

		textoFPS = "fps: 0";
		pHUD = new HUD();

#ifdef WIN32
		pFont = new Chimera::Font("C:\\Projetos\\ChimeraEngine\\fonts\\FreeSans.ttf", 18);
#else
		pFont = new Chimera::Font("../../fonts/FreeSans.ttf", 18);
#endif

		pHUD->addFont(pFont);

		SDL_Rect area = { 30, 30, 600, 20 };

		pHUD->addSquare(area, Color(1.0f, 1.0f, 1.0f, 0.2f));
		pHUD->addText(0, 0, 0, 0, Color::RED, &textoFPS);

		physicWorld = Singleton<PhysicsControl>::getRefSingleton();
	}

	GameClient::~GameClient() {

		if (pHUD)
			delete pHUD;

		Singleton<PhysicsControl>::releaseRefSingleton();
	}

	void GameClient::newFPS(const unsigned int &fps) {
		textoFPS = "fps: " + std::to_string(fps) + std::string(" Periodo: ") + std::to_string(physicWorld->getLastPeriod());
	}

	void GameClient::open() {

		deadzone = 0.02;

		FlowControl::open();

		DataMsg dataMsg(KindOp::START, this, nullptr, nullptr);
		pSceneMng->update(&dataMsg);

		pHUD->setOn(true);
	}

	void GameClient::close(void) {
		FlowControl::close();
	}

	void GameClient::processaGame() {

		physicWorld->stepSim();
		physicWorld->checkCollisions();

		FlowControl::processaGame();
	}

	void GameClient::userEvent(const SDL_Event &_event) {

		KindOp op = (KindOp)_event.user.code;
		if ((op == Chimera::KindOp::START_COLLIDE) ||
			(op == Chimera::KindOp::ON_COLLIDE) ||
			(op == Chimera::KindOp::OFF_COLLIDE)) {

			executeColisao(op, (Node*)_event.user.data1, (Node*)_event.user.data2);
		}
	}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
