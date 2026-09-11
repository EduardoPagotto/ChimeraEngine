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
        // bool pause{false};
        uint32_t beginCount{0};
        uint32_t countDelta{7};
        double ts{0.0F};

        while (!kill) {
            beginCount = SDL_GetTicks();
            while (SDL_PollEvent(&event)) {

                bool gottcha = true;
                switch (event.type) {
                    // Keyboard
                    case SDL_EVENT_KEY_DOWN:
                        if (im->keyboard.has_value()) {
                            im->keyboard->setDown(event.key);
                        }
                        break;
                    case SDL_EVENT_KEY_UP:
                        if (im->keyboard.has_value()) {
                            im->keyboard->setUp(event.key);
                        }
                        break;
                    // Mouse
                    case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    case SDL_EVENT_MOUSE_BUTTON_UP:
                        if (im->mouse.has_value()) {
                            im->mouse->updateBt(event.button);
                        }
                        break;
                    case SDL_EVENT_MOUSE_MOTION:
                        if (im->mouse.has_value()) {
                            im->mouse->updateMv(event.motion);
                        }
                        break;
                    case SDL_EVENT_MOUSE_WHEEL:
                        if (im->mouse.has_value()) {
                            im->mouse->updateWl(event.wheel);
                        }
                        break;
                    // Joystick
                    case SDL_EVENT_JOYSTICK_ADDED:
                        if (im->joystick.has_value()) {
                            im->joystick->added();
                        }
                        break;
                    case SDL_EVENT_JOYSTICK_REMOVED:
                        if (im->joystick.has_value()) {
                            im->joystick->removed(event.jdevice);
                        }
                        break;
                    // Gamepad
                    case SDL_EVENT_GAMEPAD_ADDED:
                        if (im->gamePad.has_value()) {
                            im->gamePad->added();
                        }
                        break;
                    case SDL_EVENT_GAMEPAD_REMOVED:
                        if (im->gamePad.has_value()) {
                            im->gamePad->removed(event.gdevice);
                        }
                        break;
                    // Windows
                    case SDL_EVENT_WINDOW_RESIZED: {
                        const int32_t novaWidth = event.window.data1;
                        const int32_t novaHeight = event.window.data2;
                        SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "Resize screem received: %d x %d", novaWidth, novaHeight);
                        canva->reshape(novaWidth, novaHeight);
                    } break;
                    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
                        SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "Pixel change !!");
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
                                im->paused = true;
                                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Paused Receive");
                            } break;
                            case EventCE::FLOW_RESUME: {
                                im->paused = false;
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
                                gottcha = false;
                        }
                    } break;
                    case SDL_EVENT_QUIT:
                        kill = true;
                        break;
                    default:
                        gottcha = false;
                        break;
                }

                if (im->executeEventChild || !gottcha) {
                    for (auto it = stack.end(); it != stack.begin();) {
                        if (!(*--it)->onEvent(event)) {
                            break;
                        }
                    }
                }
            }

            ts = (double)countDelta / 1000.0F;
            if (!im->paused) { // update game
                for (auto it = stack.begin(); it != stack.end(); it++) {
                    (*it)->onUpdate(ts);
                }

                canva->before();
                for (auto it = stack.begin(); it != stack.end(); it++) {
                    (*it)->onRender();
                }

                canva->after();
            }

            if (timerFPS.stepCount()) { // count FPS each second
                fps = timerFPS.getCountStep();
                sendChimeraEvent(EventCE::NEW_FPS, (void*)&fps, nullptr);
            }

            im->update();

            countDelta = SDL_GetTicks() - beginCount; // frame count limit
            if (countDelta < miniumCountDelta) {
                SDL_Delay(miniumCountDelta - countDelta);
                countDelta = miniumCountDelta;
            }
        }
    }
} // namespace ce
