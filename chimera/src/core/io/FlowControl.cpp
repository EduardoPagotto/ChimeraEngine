#include "chimera/core/io/FlowControl.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/io/JoystickManager.hpp"
#include "chimera/core/io/Keyboard.hpp"
#include "chimera/core/io/MouseDevice.hpp"
#include "chimera/core/io/utils.hpp"

namespace Chimera::Core {

FlowControl::FlowControl(IEvents* _gEvent) : gEvent(_gEvent), pause(true) {
    timerFPS.setElapsedCount(1000);
    timerFPS.start();
    JoystickManager::init();
}

FlowControl::~FlowControl() {
    JoystickManager::release();
    SDL_JoystickEventState(SDL_DISABLE);
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

bool FlowControl::changeStatusFlow(SDL_Event* pEventSDL) {

    switch (pEventSDL->user.code) {
        case Chimera::Core::EVENT_FLOW_PAUSE: {
            pause = true;
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Paused Receive");
            return false;
        } break;
        case Chimera::Core::EVENT_FLOW_RESUME: {
            pause = false;
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Resume Receive");
            return false;
        } break;
        case Chimera::Core::EVENT_FLOW_STOP: {
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
    uint32_t maxFPS = 120;
    uint32_t minimumFrameTime = 1000 / maxFPS;

    // open devices
    JoystickManager::find();
    // utilSendEvent(EVENT_FLOW_START, &layerStack, nullptr); // FIXME: remover da lista de eventos!!!!!
    gEvent->onStart();

    while (!l_quit) {
        frameTime = SDL_GetTicks();
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
                    MouseDevice::update(&l_eventSDL.button);
                case SDL_MOUSEMOTION:
                    break;
                case SDL_QUIT:
                    l_quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    break;
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

            gEvent->onEvent(l_eventSDL);
            for (std::vector<ILayer*>::iterator it = layerStack.begin(); it != layerStack.end(); it++) {
                if ((*it)->onEvent(l_eventSDL) == true)
                    break;
            }
        }
        // update game
        if (!pause) {
            try {

                for (std::vector<ILayer*>::iterator it = layerStack.begin(); it != layerStack.end(); it++) {
                    (*it)->onUpdate();
                }

                gEvent->onUpdate();
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
    JoystickManager::release();
}
} // namespace Chimera::Core