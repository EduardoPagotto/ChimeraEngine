#include "chimera/core/Engine.hpp"
#include "chimera/core/device/Keyboard.hpp"
#include "chimera/core/device/MouseDevice.hpp"
#include "chimera/core/utils.hpp"

namespace Chimera {

Engine::Engine(Canvas* canvas, const float& dist) : canvas(canvas) {
    timerFPS.setElapsedCount(1000);
    timerFPS.start();

    vp.setDist(dist);
    if (dist == 0.0f)
        vp.add("unique");
    else {
        vp.add("right");
        vp.add("right");
    }

    entity = registry.createEntity("chimera_engine", "chimera_engine");

    CanvasComponent& cc = entity.addComponent<CanvasComponent>();
    cc.canvas = canvas;

    ViewProjectionComponent& vpc = entity.addComponent<ViewProjectionComponent>();
    vpc.vp = &vp;

    SDL_Log("Engine Register: chimera_engine OK");
}

void Engine::init() {
    // CanvasComponent& cc = entity.getComponent<CanvasComponent>();
    // if (cc.canvas == nullptr)
    //     throw std::string("Engine Register: Canvas not find");

    // if (vp.size() == 0)
    //     throw std::string("Engine Register: ViewProjection not find");

    // canvas = cc.canvas;
}

void Engine::run(void) {
    SDL_Event l_eventSDL;
    bool l_quit{false}, pause{true};
    uint32_t beginCount{0}, countDelta{7}, miniumCountDelta{1000 / 140}; // 140 frames em 1000 ms
    double ts{(double)countDelta / 1000.0f};

    while (!l_quit) {
        beginCount = SDL_GetTicks();
        while (SDL_PollEvent(&l_eventSDL)) {
            switch (l_eventSDL.type) {
                case SDL_USEREVENT: {

                    switch (l_eventSDL.user.code) {
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
                    l_quit = true;
                    break;
                case SDL_WINDOWEVENT: {
                    switch (l_eventSDL.window.event) {
                        case SDL_WINDOWEVENT_RESIZED: // set windows size
                            canvas->reshape(l_eventSDL.window.data1, l_eventSDL.window.data2);
                            break;
                    }
                } break;
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
                (*it)->onUpdate(vp, ts);

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
} // namespace Chimera