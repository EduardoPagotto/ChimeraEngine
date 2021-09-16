#include "chimera/core/io/FlowControl.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/io/utils.hpp"

namespace Chimera::IO {

FlowControl::FlowControl(IEvents* _pGameClientEvents) : pGameClientEvents(_pGameClientEvents), pause(true) {
    timerFPS.setElapsedCount(1000);
    timerFPS.start();
}

FlowControl::~FlowControl() {}

bool FlowControl::changeStatusFlow(SDL_Event* pEventSDL) {

    switch (pEventSDL->user.code) {
        case Chimera::IO::EVENT_FLOW_PAUSE: {
            pause = true;
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Paused Receive");
            return false;
        } break;
        case Chimera::IO::EVENT_FLOW_RESUME: {
            pause = false;
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Resume Receive");
            return false;
        } break;
        case Chimera::IO::EVENT_FLOW_STOP: {
            SDL_Event l_eventQuit;
            l_eventQuit.type = SDL_QUIT;
            if (SDL_PushEvent(&l_eventQuit) == -1) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Critical SDL_QUIT PushEvent fail: %s", SDL_GetError());
            }
        } break;
        default:
            break;
    }

    return true;
}

void FlowControl::run(void) {
    SDL_Event l_eventSDL;
    bool l_quit = false;
    uint32_t frameTime, timeElapsed, tot_delay;
    uint32_t lastFrameTime = 0;
    uint32_t deltaTime = 0;
    uint32_t fpsMin = 60;
    uint32_t minimumFrameTime = 1000 / fpsMin;

    // open devices
    joystickManager.Initialize();
    joystickManager.FindJoysticks();
    utilSendEvent(EVENT_FLOW_START, nullptr, nullptr);

    while (!l_quit) {
        frameTime = SDL_GetTicks();
        while (SDL_PollEvent(&l_eventSDL)) {
            switch (l_eventSDL.type) {
                case SDL_USEREVENT:
                    if (changeStatusFlow(&l_eventSDL))
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
        // update game
        if (!pause) {
            try {
                pGameClientEvents->update();
            } catch (...) { SDL_Quit(); }
        }
        // count frame and FPS
        if (timerFPS.stepCount() == true) {
            fps = timerFPS.getCountStep();
            utilSendEvent(EVENT_NEW_FPS, (void*)&fps, nullptr);
        }
        // counters temps
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
    // Release devices
    joystickManager.ReleaseJoysticks();
}
} // namespace Chimera::IO