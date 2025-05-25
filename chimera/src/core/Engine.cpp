#include "chimera/core/Engine.hpp"
#include "chimera/base/ServiceLocator.hpp"
#include "chimera/base/event.hpp"

namespace ce {

    Engine::Engine() {
        timerFPS.setElapsedCount(1000);
        timerFPS.start();

        canvas = g_service_locator.getService<ICanva>();
        SDL_Log("Engine Register: chimera_engine OK");
    }

    Engine::~Engine() { canvas.reset(); }

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

                        switch (static_cast<EventCE>(event.user.code)) {
                            case EventCE::FLOW_PAUSE: {
                                pause = true;
                                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Paused Receive");
                            } break;
                            case EventCE::FLOW_RESUME: {
                                pause = false;
                                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Resume Receive");
                            } break;
                            case EventCE::FLOW_STOP: {
                                SDL_Event l_eventQuit;
                                l_eventQuit.type = SDL_QUIT;
                                if (SDL_PushEvent(&l_eventQuit) == -1) {
                                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Critical SDL_QUIT PushEvent fail: %s",
                                                 SDL_GetError());
                                }
                            } break;
                            case EventCE::TOGGLE_FULL_SCREEN:
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
                    (*it)->onUpdate(ts);

                canvas->before();
                for (auto it = stack.begin(); it != stack.end(); it++)
                    (*it)->onRender();

                canvas->after();
            }

            if (timerFPS.stepCount() == true) { // count FPS each second
                fps = timerFPS.getCountStep();
                sendChimeraEvent(EventCE::NEW_FPS, (void*)&fps, nullptr);
            }

            countDelta = SDL_GetTicks() - beginCount; // frame count limit
            if (countDelta < miniumCountDelta) {
                SDL_Delay(miniumCountDelta - countDelta);
                countDelta = miniumCountDelta;
            }
        }
    }
} // namespace ce
