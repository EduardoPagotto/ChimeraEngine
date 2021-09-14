#include "chimera/core/io/FlowControl.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/io/utils.hpp"

namespace Chimera::IO {

FlowControl::FlowControl(IEvents* _pGameClientEvents) : pGameClientEvents(_pGameClientEvents) {
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
        fps = timerFPS.getCountStep();
        utilSendEvent(EVENT_NEW_FPS, (void*)&fps, nullptr);
    }
}

void FlowControl::processaGame() {

    try {
        countFrame();
        pGameClientEvents->render();
    } catch (...) { SDL_Quit(); }
}

void FlowControl::gameLoop(void) {
    SDL_Event l_eventSDL;
    bool l_quit = false;
    bool l_isActive = false;
    unsigned int frameTime;
    unsigned int timeElapsed;
    unsigned int tot_delay;
    unsigned int lastFrameTime = 0;
    unsigned int deltaTime = 0;
    unsigned int fpsMin = 60;
    unsigned int minimumFrameTime = 1000 / fpsMin;

    while (!l_quit) {

        frameTime = SDL_GetTicks();

        while (SDL_PollEvent(&l_eventSDL)) {

            switch (l_eventSDL.type) {
                case SDL_USEREVENT:
                    pGameClientEvents->userEvent(l_eventSDL);
                    break;
                case SDL_KEYDOWN:
                    pGameClientEvents->keboardEvent(l_eventSDL.key.keysym.sym);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    mouse.update(&l_eventSDL.button);
                case SDL_MOUSEMOTION:
                    pGameClientEvents->mouseEvent(&mouse, &l_eventSDL);
                    break;
                case SDL_QUIT:
                    l_quit = true;
                    pGameClientEvents->stop();
                    break;
                case SDL_WINDOWEVENT:
                    pGameClientEvents->windowEvent(l_eventSDL.window);
                    break;
                case SDL_JOYAXISMOTION: {
                    JoystickState* pJoy = joystickManager.setAxisMotion(&l_eventSDL.jaxis);
                    pGameClientEvents->joystickEvent(pJoy, &l_eventSDL);
                } break;
                case SDL_JOYBUTTONDOWN:
                case SDL_JOYBUTTONUP: {
                    JoystickState* pJoy = joystickManager.setButtonState(&l_eventSDL.jbutton);
                    pGameClientEvents->joystickEvent(pJoy, &l_eventSDL);
                } break;
                case SDL_JOYHATMOTION: {
                    JoystickState* pJoy = joystickManager.setHatMotion(&l_eventSDL.jhat);
                    pGameClientEvents->joystickEvent(pJoy, &l_eventSDL);
                } break;
                case SDL_JOYBALLMOTION: {
                    JoystickState* pJoy = joystickManager.setBallMotion(&l_eventSDL.jball);
                    pGameClientEvents->joystickEvent(pJoy, &l_eventSDL);
                } break;
                case SDL_JOYDEVICEADDED:
                case SDL_JOYDEVICEREMOVED:
                    joystickManager.FindJoysticks();
                default:
                    break;
            }
        }

        if (pGameClientEvents->paused() == false)
            processaGame(); // Se nao houver foco na tela pule o render

        // inicio contadores
        deltaTime = frameTime - lastFrameTime;
        lastFrameTime = frameTime;

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
} // namespace Chimera::IO