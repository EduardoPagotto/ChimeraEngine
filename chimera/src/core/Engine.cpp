#include "chimera/core/Engine.hpp"
#include "chimera/core/device/JoystickManager.hpp"
#include "chimera/core/device/Keyboard.hpp"
#include "chimera/core/device/MouseDevice.hpp"
#include "chimera/core/utils.hpp"

namespace Chimera {

Engine::Engine() : canvas(nullptr), pause(true), fps(140) {
    timerFPS.setElapsedCount(1000);
    timerFPS.start();
    JoystickManager::init();
}

Engine::~Engine() {
    JoystickManager::release();
    SDL_JoystickEventState(SDL_DISABLE);
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

void Engine::init() {
    CanvasComponent& cc = registry.findComponent<CanvasComponent>("main_screem");
    canvas = cc.canvas;
}

bool Engine::changeStatusFlow(SDL_Event* pEventSDL) {

    switch (pEventSDL->user.code) {
        case EVENT_FLOW_PAUSE: {
            pause = true;
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Paused Receive");
        } break;
        case EVENT_FLOW_RESUME: {
            pause = false;
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Resume Receive");
        } break;
        case EVENT_FLOW_STOP: {
            SDL_Event l_eventQuit;
            l_eventQuit.type = SDL_QUIT;
            if (SDL_PushEvent(&l_eventQuit) == -1) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Critical SDL_QUIT PushEvent fail: %s", SDL_GetError());
            }
        } break;
        case EVENT_TOGGLE_FULL_SCREEN:
            canvas->toggleFullScreen();
            break;
        default:
            break;
    }

    return true;
}

void Engine::run(void) {
    SDL_Event l_eventSDL;
    bool l_quit = false;
    uint32_t beginCount = 0, countDelta = 7, miniumCountDelta = 1000 / 140; // 140 frames em 1000 ms
    double ts = (double)countDelta / 1000.0f;
    JoystickManager::find(); // open devices

    while (!l_quit) {
        beginCount = SDL_GetTicks();
        while (SDL_PollEvent(&l_eventSDL)) {
            switch (l_eventSDL.type) {
                case SDL_USEREVENT:
                    if (!changeStatusFlow(&l_eventSDL))
                        continue;
                    break;
                case SDL_KEYDOWN:
                    Keyboard::setDown(l_eventSDL.key);
                    break;
                case SDL_KEYUP:
                    Keyboard::setUp(l_eventSDL.key);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    MouseDevice::updateBt(&l_eventSDL.button);
                    break;
                case SDL_MOUSEMOTION:
                    MouseDevice::updateMv(&l_eventSDL.motion);
                    break;
                case SDL_MOUSEWHEEL:
                    MouseDevice::updateWl(&l_eventSDL.wheel);
                    break;
                case SDL_QUIT:
                    l_quit = true;
                    break;
                case SDL_WINDOWEVENT: {
                    switch (l_eventSDL.window.event) {
                        case SDL_WINDOWEVENT_RESIZED: // set windows size
                            canvas->reshape(l_eventSDL.window.data1, l_eventSDL.window.data2);
                            break;
                    }
                } break;
                case SDL_JOYAXISMOTION:
                    JoystickManager::setAxisMotion(&l_eventSDL.jaxis);
                    break;
                case SDL_JOYBUTTONDOWN:
                case SDL_JOYBUTTONUP:
                    JoystickManager::setButtonState(&l_eventSDL.jbutton);
                    break;
                case SDL_JOYHATMOTION:
                    JoystickManager::setHatMotion(&l_eventSDL.jhat);
                    break;
                case SDL_JOYBALLMOTION:
                    JoystickManager::setBallMotion(&l_eventSDL.jball);
                    break;
                case SDL_JOYDEVICEADDED:
                case SDL_JOYDEVICEREMOVED:
                    JoystickManager::find();
                default:
                    break;
            }

            for (auto it = stack.end(); it != stack.begin();) {
                if ((*--it)->onEvent(l_eventSDL) == false)
                    break;
            }
        }

        ts = (double)countDelta / 1000.0f;
        if (!pause) { // update game
            for (auto it = stack.begin(); it != stack.end(); it++)
                (*it)->onUpdate(vpo, ts);

            canvas->before();
            for (auto it = stack.begin(); it != stack.end(); it++)
                (*it)->onRender();

            canvas->after();
        }

        if (timerFPS.stepCount() == true) { // count FPS each second
            fps = timerFPS.getCountStep();
            utilSendEvent(EVENT_NEW_FPS, (void*)&fps, nullptr);
        }

        countDelta = SDL_GetTicks() - beginCount; // frame count limit
        if (countDelta < miniumCountDelta) {
            SDL_Delay(miniumCountDelta - countDelta);
            countDelta = miniumCountDelta;
        }
    }
    JoystickManager::release(); // Release devices
}
} // namespace Chimera