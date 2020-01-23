#include "chimera/core/FlowControl.hpp"
#include "chimera/core/Exception.hpp"

namespace Chimera {

FlowControl::FlowControl(IGameClientEvents* _pGameClientEvents) : pGameClientEvents(_pGameClientEvents) {
    timerFPS.setElapsedCount(1000);
    timerFPS.start();
}

FlowControl::~FlowControl() {}

void FlowControl::open() {
    joystickManager.Initialize();
    joystickManager.FindJoysticks();
    pGameClientEvents->start();
}

void FlowControl::close(void) {

    joystickManager.ReleaseJoysticks();

    SDL_Event l_eventQuit;
    l_eventQuit.type = SDL_QUIT;
    if (SDL_PushEvent(&l_eventQuit) == -1) {
        throw Exception(std::string(SDL_GetError()));
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
    } catch (...) { SDL_Quit(); }
}

void FlowControl::gameLoop(void) {

    SDL_Event l_eventSDL;
    bool l_quit = false;
    bool l_isActive = false;

    unsigned int frameTime;
    unsigned int lastFrameTime = 0;
    unsigned int deltaTime = 0;
    // unsigned int fps = 60;
    // unsigned int minimumFrameTime = 1000 // fps;
    unsigned int minimumFrameTime = 16; // 1000/16
    unsigned int timeElapsed;
    unsigned int tot_delay;

    while (!l_quit) {

        frameTime = SDL_GetTicks();

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

        // inicio contadores
        deltaTime = frameTime - lastFrameTime;
        lastFrameTime = frameTime;

        if (pGameClientEvents->paused() == false)
            processaGame(); // Se nao houver foco na tela pule o render

        timeElapsed = (SDL_GetTicks() - frameTime);
        if (timeElapsed < minimumFrameTime) {
            tot_delay = minimumFrameTime - timeElapsed;
            // SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "DeltaTime: %d  Delay: %d", deltaTime, tot_delay);
            SDL_Delay(tot_delay);
        } else {
            // SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "DeltaTime: %d TimeElapsed: %d", deltaTime, timeElapsed);
        }
    }
}
} // namespace Chimera