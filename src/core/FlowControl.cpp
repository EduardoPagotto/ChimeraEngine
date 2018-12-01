#include "chimera/core/FlowControl.hpp"
#include "chimera/core/DataMsg.hpp"
#include "chimera/core/ExceptionChimera.hpp"
//#include <iostream>

namespace Chimera {

FlowControl::FlowControl(IGameClientEvents* _pGameClientEvents) : pGameClientEvents(_pGameClientEvents) {
    timerFPS.setElapsedCount(1000);
    timerFPS.start();
    log = spdlog::get("chimera");
    log->debug("Constructor FlowControl");
}

FlowControl::~FlowControl() { log->debug("Destructor FlowControl"); }

void FlowControl::open() {

    joystickManager.Initialize();
    joystickManager.FindJoysticks();
    log->debug("FlowControl Joystick: {0}", joystickManager.GetStatusManager());

    pGameClientEvents->start();
}

void FlowControl::close(void) {

    joystickManager.ReleaseJoysticks();

    SDL_Event l_eventQuit;
    l_eventQuit.type = SDL_QUIT;
    if (SDL_PushEvent(&l_eventQuit) == -1) {
        throw ExceptionChimera(std::string(SDL_GetError()));
    }
}

void FlowControl::countFrame() {
    if (timerFPS.stepCount() == true) {
        unsigned int fps = timerFPS.getCountStep();
        pGameClientEvents->newFPS(fps);
    }
}

void FlowControl::processaGame() {

    try {
        countFrame();
        pGameClientEvents->render();
        pGameClientEvents->joystickStatus(joystickManager);
    } catch (...) {
        log->error("Erro Gravissimo:");
        SDL_Quit();
    }
}

void FlowControl::gameLoop(void) {

    SDL_Event l_eventSDL;
    bool l_quit = false;
    bool l_isActive = false;

    while (!l_quit) {

        while (SDL_PollEvent(&l_eventSDL)) {

            switch (l_eventSDL.type) {
                case SDL_USEREVENT:
                    pGameClientEvents->userEvent(l_eventSDL);
                    break;
                case SDL_KEYDOWN:
                    pGameClientEvents->keyCapture(l_eventSDL.key.keysym.sym);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    pGameClientEvents->mouseButtonDownCapture(l_eventSDL.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    pGameClientEvents->mouseButtonUpCapture(l_eventSDL.button);
                    break;
                case SDL_MOUSEMOTION:
                    pGameClientEvents->mouseMotionCapture(l_eventSDL.motion);
                    break;
                case SDL_QUIT:
                    log->warn("FlowControl QUIT Sinalizado");
                    l_quit = true;
                    pGameClientEvents->stop();
                    break;
                case SDL_WINDOWEVENT:
                    pGameClientEvents->windowEvent(l_eventSDL.window);
                    break;
                default:
                    if (joystickManager.TrackEvent(&l_eventSDL) == true) {
                        pGameClientEvents->joystickCapture(joystickManager);
                    }
                    break;
            }
        }

        if (pGameClientEvents->paused() == false) {
            // Se nao houver foco na tela pule o render
            processaGame();
        }
    }
}
} // namespace Chimera