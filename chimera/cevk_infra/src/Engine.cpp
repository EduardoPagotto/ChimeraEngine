#include "cevk_infra/Engine.hpp"
#include "cevk_infra/event.hpp"

namespace ce {

    Engine::Engine(std::shared_ptr<IScr> screen) : screen(screen) {

        timerFPS.setElapsedCount(1000);
        timerFPS.start();

        SDL_Log("Engine Register: chimera_engine OK");
    }

    void Engine::run() {

        SDL_Event event;
        bool kill{false};
        bool pause{false};
        uint32_t beginCount{0};
        uint32_t countDelta{7};
        double ts{0.0F};

        while (!kill) {
            beginCount = SDL_GetTicks();
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_EVENT_USER: {

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
                                l_eventQuit.type = SDL_EVENT_QUIT;
                                if (!SDL_PushEvent(&l_eventQuit)) {
                                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Critical SDL_QUIT PushEvent fail: %s",
                                                 SDL_GetError());
                                }
                            } break;
                            case EventCE::TOGGLE_FULL_SCREEN:
                                screen->toggleFullScreen();
                                break;
                            default:
                                break;
                        }
                    }

                    break;
                    case SDL_EVENT_QUIT:
                        kill = true;
                        break;
                    case SDL_EVENT_WINDOW_RESIZED: {
                        screen->reshape(event.window.data1, event.window.data2);
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

                screen->before();
                for (auto it = stack.begin(); it != stack.end(); it++)
                    (*it)->onRender();

                screen->after();
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
