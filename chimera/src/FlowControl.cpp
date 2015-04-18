#include "FlowControl.h"
#include "ExceptionSDL.h"
#include <iostream>

namespace Chimera {

	FlowControl::FlowControl(Video* _pVideo) : pVideo(_pVideo)
	{
		timerFPS.setElapsedCount(1000);
		timerFPS.start();
	}

	FlowControl::~FlowControl()
	{

	}

	void FlowControl::open() {

		joystickManager.Initialize();
		joystickManager.FindJoysticks();
		std::cout << "Joystick: " << joystickManager.GetStatusManager() << std::endl;

		pVideo->initGL();

		start();
	}

	void FlowControl::close(void) {

		SDL_Event l_eventQuit;
		l_eventQuit.type = SDL_QUIT;
		if (SDL_PushEvent(&l_eventQuit) == -1) {
			throw ExceptionSDL(ExceptionCode::CLOSE, std::string(SDL_GetError()));
		}
	}

	void FlowControl::countFrame() {
		if (timerFPS.stepCount() == true) {
			unsigned int fps = timerFPS.getCountStep();
			newFPS(fps);
		}
	}

	void FlowControl::processaGame() {
		countFrame();
		pVideo->initDraw();
		render();
		pVideo->endDraw();
		joystickStatus(joystickManager);
	}

	void FlowControl::gameLoop(void) {

		SDL_Event l_eventSDL;
		bool l_quit = false;
		bool l_isActive = false;

		while (!l_quit) {

			while (SDL_PollEvent(&l_eventSDL)) {

				switch (l_eventSDL.type) {
				case SDL_USEREVENT:
					userEvent(l_eventSDL);
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