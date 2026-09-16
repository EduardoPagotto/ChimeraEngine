#include "chimera_base/Engine.hpp"
#include "chimera_base/InputManager.hpp"
#include "chimera_base/event.hpp"

namespace ce {

    Engine::Engine(std::shared_ptr<entt::registry> registry, std::shared_ptr<ICanva> canva)
        : registry(registry), canva(canva) {

        timerFPS.setElapsedCount(1000);
        timerFPS.start();

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Engine Chimera OK");
    }

    void Engine::run() { // NOLINT

        auto& im = registry->ctx().get<std::shared_ptr<InputManager>>();

        SDL_Event event;
        bool kill{false};
        uint32_t beginCount{0};
        uint32_t countDelta{7};
        double ts{0.0F};

        while (!kill) {

            beginCount = SDL_GetTicks();

            im->startFrame();

            while (SDL_PollEvent(&event)) {

                switch (event.type) {
                    // Windows
                    case SDL_EVENT_WINDOW_RESIZED: {
                        const int32_t novaWidth = event.window.data1;
                        const int32_t novaHeight = event.window.data2;
                        SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "Resize screem received: %d x %d", novaWidth, novaHeight);
                        canva->reshape(novaWidth, novaHeight);
                    } break;
                    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
                        int newWidth = event.window.data1;
                        int newHeight = event.window.data2;

                        SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "Pixel change (%d x %d)", newWidth, newHeight);
                    } break;
                    case SDL_EVENT_WINDOW_MAXIMIZED:
                    case SDL_EVENT_WINDOW_RESTORED: {
                        sendChimeraEvent(EventCE::FLOW_RESUME, nullptr, nullptr);
                        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Windows restored/maximized");
                    } break;
                    case SDL_EVENT_WINDOW_MINIMIZED: {
                        sendChimeraEvent(EventCE::FLOW_PAUSE, nullptr, nullptr);
                        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Windows minimized");
                    } break;
                    // User
                    case SDL_EVENT_USER: {
                        switch (static_cast<EventCE>(event.user.code)) {
                            case EventCE::FLOW_PAUSE: {
                                im->setStatusPause(true);
                                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Paused Receive");
                            } break;
                            case EventCE::FLOW_RESUME: {
                                im->setStatusPause(false);
                                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Resume Receive");
                            } break;
                            case EventCE::FLOW_STOP: {
                                SDL_Event l_eventQuit;
                                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "QUIT Receive");
                                l_eventQuit.type = SDL_EVENT_QUIT;
                                if (!SDL_PushEvent(&l_eventQuit)) {
                                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Critical SDL_QUIT PushEvent fail: %s",
                                                 SDL_GetError());
                                }
                            } break;
                            case EventCE::TOGGLE_FULL_SCREEN:
                                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Toggle fullscreem received");
                                canva->toggleFullScreen();
                                break;
                            default:
                                break;
                        }
                    } break;
                    case SDL_EVENT_QUIT:
                        kill = true;
                        break;
                    default:
                        break;
                }

                im->handleEvent(event);

                for (auto& ev : stack) {
                    ev->onEvent(event);
                }
            }

            // Atualiza o estado das teclas que continuam pressionadas
            im->updateContinuousInput();

            ts = (double)countDelta / 1000.0F;
            if (!im->getStatusPause()) { // update game

                for (auto iten : stack) {
                    iten->onUpdate(ts);
                }

                canva->before();

                for (auto iten : stack) {
                    iten->onRender();
                }

                canva->after();
            }

            if (timerFPS.stepCount()) { // count FPS each second
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
