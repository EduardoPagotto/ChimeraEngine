#include "chimera/core/Engine.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/ServiceLocator.hpp"
#include "chimera/core/utils.hpp"

namespace ce {

Engine::Engine() {
    timerFPS.setElapsedCount(1000);
    timerFPS.start();

    canvas = g_service_locator.getService<ICanva>();
    vp = g_service_locator.getService<IViewProjection>();
    Entity entity = g_registry.createEntity("chimera_engine", "chimera_engine");

    SDL_Log("Engine Register: chimera_engine OK");
}

Engine::~Engine() {
    canvas = nullptr;
    vp = nullptr;
}

void Engine::run(void) {
    SDL_Event event;
    bool kill{false}, pause{true};
    uint32_t beginCount{0}, countDelta{7}, miniumCountDelta{1000 / 140}; // 140 frames em 1000 ms
    double ts{(double)countDelta / 1000.0f};

    while (!kill) {
        beginCount = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_USEREVENT: {

                    switch (event.user.code) {
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
                }

                break;
                case SDL_QUIT:
                    kill = true;
                    break;
                case SDL_WINDOWEVENT: {
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED: // set windows size
                            canvas->reshape(event.window.data1, event.window.data2);
                            break;
                    }
                } break;
                default:
                    break;
            }

            for (auto it = stack.end(); it != stack.begin();) {
                if ((*--it)->onEvent(event) == false)
                    break;
            }
        }

        ts = (double)countDelta / 1000.0f;
        if (!pause) { // update game
            for (auto it = stack.begin(); it != stack.end(); it++)
                (*it)->onUpdate(*vp.get(), ts);

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
}
} // namespace ce