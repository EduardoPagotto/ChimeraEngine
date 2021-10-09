#include "chimera/core/Application.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/io/JoystickManager.hpp"
#include "chimera/core/io/Keyboard.hpp"
#include "chimera/core/io/MouseDevice.hpp"
#include "chimera/core/io/utils.hpp"

namespace Chimera {

Application::Application(Canvas* canvas) : canvas(canvas), pause(true) {
    timerFPS.setElapsedCount(1000);
    timerFPS.start();
    JoystickManager::init();
}

Application::~Application() {
    for (ILayer* l : layerStack) { // TODO: melhorar remover apenas os existentes (fazer o deatach!!)
        l->onDeatach();
        delete l;
    }
    layerStack.clear();
    JoystickManager::release();
    SDL_JoystickEventState(SDL_DISABLE);
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);

    delete canvas;
}

bool Application::changeStatusFlow(SDL_Event* pEventSDL) {

    switch (pEventSDL->user.code) {
        case Chimera::EVENT_FLOW_PAUSE: {
            pause = true;
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Paused Receive");
            return false;
        } break;
        case Chimera::EVENT_FLOW_RESUME: {
            pause = false;
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Resume Receive");
            return false;
        } break;
        case Chimera::EVENT_FLOW_STOP: {
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

void Application::pushLayer(ILayer* layer) {
    layerStack.pushLayer(layer);
    layer->onAttach();
}

void Application::pushOverlay(ILayer* overlay) {
    layerStack.pushOverlay(overlay);
    overlay->onAttach();
}

void Application::run(void) {
    SDL_Event l_eventSDL;
    bool l_quit = false;
    uint32_t frameTime, timeElapsed, tot_delay;
    uint32_t lastFrameTime = 0;
    uint32_t deltaTime = 0;
    uint32_t maxFPS = 120;
    uint32_t minimumFrameTime = 1000 / maxFPS;

    // open devices
    JoystickManager::find();
    this->onStart();

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

            this->onEvent(l_eventSDL);
            for (auto it = layerStack.end(); it != layerStack.begin();) {
                if ((*--it)->onEvent(l_eventSDL) == true)
                    break;
            }
        }
        // update game
        if (!pause) {
            try {

                for (auto it = layerStack.begin(); it != layerStack.end(); it++)
                    (*it)->onUpdate();

                this->onUpdate();
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
} // namespace Chimera