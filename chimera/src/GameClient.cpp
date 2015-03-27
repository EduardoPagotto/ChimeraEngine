#include "GameClient.h"
#include "Singleton.h"

#include <iostream>

namespace Chimera {

	GameClient::GameClient(Video *_pVideo, Chimera::SceneMng *_pScenMng) : pSceneMng(_pScenMng), pVideo(_pVideo)  {

		textoFPS = "fps: " + std::to_string(0);
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

		fps = 0;
		timerFPS.setElapsedCount(1000);
		timerFPS.start();

		timerSegundo.setElapsedCount(1000);
		timerSegundo.start();

		physicWorld = Singleton<PhysicsControl>::getRefSingleton();
	}

	GameClient::~GameClient() {

		if (pHUD)
			delete pHUD;

		Singleton<PhysicsControl>::releaseRefSingleton();
	}

	bool GameClient::newFPS() {
		return timerSegundo.stepCount();
	}

	void GameClient::countFrame() {
		if (timerFPS.stepCount() == true) {
			fps = timerFPS.getCountStep();
			textoFPS = "fps: " + std::to_string(fps) + std::string(" Periodo: ") + std::to_string(physicWorld->getLastPeriod());
		}
	}

	void GameClient::open() {

        deadzone = 0.02;
		joystickManager.Initialize();
		joystickManager.FindJoysticks();
		std::cout << "Joystick: " << joystickManager.GetStatusManager() << std::endl;

		pVideo->initGL();

		start();

		DataMsg dataMsg(KindOp::START, this, nullptr, nullptr);
		pSceneMng->update(&dataMsg);

		pHUD->setOn(true);
	}

	void GameClient::close(void) {

		SDL_Event l_eventQuit;
		l_eventQuit.type = SDL_QUIT;
		if (SDL_PushEvent(&l_eventQuit) == -1) {
			throw ExceptionSDL(ExceptionCode::CLOSE, std::string(SDL_GetError()));
		}

	}

	void GameClient::processaGame() {

		physicWorld->stepSim();
		physicWorld->checkCollisions();

		countFrame();

		pVideo->initDraw();

		render();

		pVideo->endDraw();
	}

	void GameClient::validaOpColisao(const SDL_Event &_event) {

		KindOp op = (KindOp)_event.user.code;

		if ((op == Chimera::KindOp::START_COLLIDE) ||
			(op == Chimera::KindOp::ON_COLLIDE) ||
			(op == Chimera::KindOp::OFF_COLLIDE)) {

			executeColisao(op, (Node*)_event.user.data1, (Node*)_event.user.data2);

		}
		else {
			userEvent(_event);
		}

	}

	void GameClient::gameLoop(void) {

		SDL_Event l_eventSDL;
		bool l_quit = false;
		bool l_isActive = false;

		while (!l_quit) {

			while (SDL_PollEvent(&l_eventSDL)) {

				switch (l_eventSDL.type) {
				case SDL_USEREVENT:
					validaOpColisao(l_eventSDL);
					break;
				case SDL_KEYDOWN:
					keyCapture(l_eventSDL.key.keysym.sym);
					break;
				case SDL_MOUSEBUTTONDOWN:
					mouseButtonDownCapture(l_eventSDL.button);
					break;
				case SDL_MOUSEBUTTONUP:
					mouseButtonUpCapture(l_eventSDL.button);
					break;
				case SDL_MOUSEMOTION:
					mouseMotionCapture(l_eventSDL.motion);
					break;
				case SDL_QUIT:
					l_quit = true;
					stop();
					break;
				case SDL_WINDOWEVENT: {
										  switch (l_eventSDL.window.event) {
										  case SDL_WINDOWEVENT_ENTER:
											  l_isActive = true;
											  break;
										  case SDL_WINDOWEVENT_LEAVE:
											  l_isActive = false;
											  break;
										  case SDL_WINDOWEVENT_RESIZED:
											  pVideo->reshape(l_eventSDL.window.data1, l_eventSDL.window.data2);
											  break;
										  default:
											  break;
										  }
				}
					break;
				default:
					if (joystickManager.TrackEvent(&l_eventSDL) == true)
						joystickCapture(joystickManager);
					break;
				}

			}

			//if (l_isActive==true) {
			//Se nao houver foco na tela pule o render
			processaGame();

			//}

		}
	}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
